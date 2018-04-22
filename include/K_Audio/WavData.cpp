#include"WavData.h"

namespace K_Audio {
	////////
	//public
	////

	WavData::WavData(const char* filePass) {
		if (!LoadFile(filePass)) {
			throw("wavData Initialize Failed : " + std::string(filePass));
		}
	}

	WavData::~WavData() {
		this->waveFile.close();
	}

	void WavData::Seek(int offset) {
		//ファイルの先頭からPCMまでの位置まで移動させたあとに (offset * blockSize) バイト分移動
		PcmSeek(offset);
		this->pcmOffset = offset;

		//終端を超えたら終端位置にシーク
		if (this->loopLength < this->pcmOffset) {
			this->pcmOffset = this->loopLength;
			PcmSeek(this->loopLength);
		}
	}

	//maxSize分読む
	int WavData::Read(char* buffer, int maxSize) {
		//読み込みサイズがPCMの一つ分サイズで割り切れないなら失敗
		if (maxSize % this->blockSize != 0) {
			return 0;
		}
		int readSize = maxSize;
		//終端を超えないようにサイズ調整
		if (this->loopStart + this->loopLength < this->pcmOffset + readSize / this->blockSize) {
			readSize = (this->loopLength - this->pcmOffset) * this->blockSize;
		}

		if (readSize == 0) {
			return 0;
		}

		int prevOffset = this->pcmOffset;
		this->waveFile.read(buffer, readSize);
		this->pcmOffset += readSize / this->blockSize;

		//実際に読み込んだサイズを求めて返す
		return (this->pcmOffset - prevOffset) * this->blockSize;
	}


	////////
	//private
	////

	//読み込み
	bool WavData::LoadFile(const char* filePass) {
		//各チャンクの先頭にこの情報が必ずある
		struct WaveChunk {
			char id[4];
			int size;
		};
		//fmtチャンク用の構造体
		struct WaveStruct {
			unsigned short formatId;           //フォーマットID
			unsigned short numChannel;     //チャンネル数 monaural=1 , stereo=2
			unsigned long  samplingRate;     //１秒間のサンプル数，サンプリングレート(Hz)
			unsigned long  bytesPerSec;       //１秒間のデータサイズ
			unsigned short blockSize;          //１ブロックのサイズ．8bit:nomaural=1byte , 16bit:stereo=4byte
			unsigned short bitsPerSample;     //１サンプルのビット数 8bit or 16bit
		};


		this->waveFile.open(filePass, std::ifstream::binary);
		if (!this->waveFile) {
			return false;
		}
		//RIFFチャンクの先頭12バイト
		//id = 4bite : size = 4bite;
		WaveChunk chunk;
		WaveStruct fmtChunk;
		char format[4];

		this->waveFile.read((char*)&chunk, 8);
		this->waveFile.read(format, 4);

		//WAVEフォーマット以外は失敗
		if (strncmp(chunk.id, "RIFF", 4) != 0 || strncmp(format, "WAVE", 4)) {
			return false;
		}

		//各チャンクを読む
		int count = 0;
		int byteOffset = 12;
		int dataSize = 0;
		int fileSize = chunk.size;
		while (byteOffset < fileSize) {
			this->waveFile.read((char*)&chunk, sizeof(WaveChunk));
			if (strncmp(chunk.id, "fmt ", 4) == 0) {
				//fmtチャンク
				this->waveFile.read((char*)&fmtChunk, chunk.size);
				if (fmtChunk.formatId != 1) {
					return false;
				}
				byteOffset += chunk.size + sizeof(WaveChunk);
				++count;
			}
			else if (strncmp(chunk.id, "data ", 4) == 0) {
				//dataチャンク
				//位置を保存して逐次読み込み用のデータに利用
				this->dataStartOffset = byteOffset + sizeof(WaveChunk);
				this->waveFile.seekg(chunk.size, std::ios_base::cur);
				byteOffset += chunk.size + sizeof(WaveChunk);
				dataSize = chunk.size;
				++count;
			}
			else {
				//それ以外のチャンクはスキップ
				this->waveFile.seekg(chunk.size, std::ios_base::cur);
				byteOffset += chunk.size + sizeof(WaveChunk);
			}
		}

		//fmt,dataのチャンク合わせて２つでない時は失敗
		if (count != 2) {
			return false;
		}

		this->samplingRate = fmtChunk.samplingRate;
		if (fmtChunk.numChannel == 1) {
			//モノラル
			if (fmtChunk.bitsPerSample == 8) {
				this->format = SoundFormat::Mono8;
			};
			if (fmtChunk.bitsPerSample == 16) {
				this->format = SoundFormat::Mono16;
			};
		}
		else {
			//ステレオ
			if (fmtChunk.bitsPerSample == 8) {
				this->format = SoundFormat::Stereo8;
			};
			if (fmtChunk.bitsPerSample == 16) {
				this->format = SoundFormat::Stereo16;
			};
		}

		this->blockSize = fmtChunk.blockSize;

		//oggと違ってこっちはループポイントが最初と最後で固定
		this->loopStart = 0;
		this->loopLength = this->loopStart + dataSize / this->blockSize;

		Seek(0);
		return true;
	}

	//WaveファイルのPCMデータのシーク
	void WavData::PcmSeek(int pcmOffset) {
		this->waveFile.seekg(this->dataStartOffset + pcmOffset * this->blockSize, std::ios_base::beg);
	}
}
