#include"WavData.h"

namespace K_Audio {
	////////
	//public
	////

	WavData::WavData(const char* filePass) {
		try {
			LoadFile(filePass);
		}
		catch (std::exception& e) {
			throw e;
		}
	}

	WavData::~WavData() {
		if (this->wavData != nullptr) {
			delete[] this->wavData;
		}
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

		//読み込み
		int prevOffset = this->pcmOffset;
		memcpy(buffer, this->wavData + this->binaryPosition, readSize);
		this->binaryPosition += readSize;
		this->pcmOffset += readSize / this->blockSize;

		//実際に読み込んだサイズを求めて返す
		return (this->pcmOffset - prevOffset) * this->blockSize;
	}


	////////
	//private
	////

	//読み込み
	void WavData::LoadFile(const char* filePass) {
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

		//値初期化
		this->wavData = nullptr;
		this->binaryPosition = 0;
		this->binarySize = 0;

		//ファイル読み込み
		std::ifstream ifs(filePass, std::ios::binary | std::ios::in);
		if (!ifs) {
			throw std::runtime_error("fileOpen failed : " + std::string(filePass));
		}
		//ファイルサイズ取得
		ifs.seekg(0, std::ios::end);
		this->binarySize = ifs.tellg();
		ifs.clear();
		ifs.seekg(0, std::ios::beg);

		//バイナリを全部読みこみ
		this->wavData = new char[binarySize];
		ifs.read(this->wavData, binarySize);
		ifs.close();

		//RIFFチャンクの先頭12バイト
		//id = 4bite : size = 4bite;
		WaveChunk chunk;
		WaveStruct fmtChunk;
		char format[4];

		ReadBinary((char*)&chunk, this->wavData, &this->binaryPosition, 8);
		ReadBinary(format, this->wavData, &this->binaryPosition, 4);

		//WAVEフォーマット以外は失敗
		if (strncmp(chunk.id, "RIFF", 4) != 0 || strncmp(format, "WAVE", 4)) {
			throw std::runtime_error("illegal waveFormat (this is not WAVE format) : " + std::string(filePass));
		}

		//各チャンクを読む
		int count = 0;
		int byteOffset = 12;
		int dataSize = 0;
		int fileSize = chunk.size;
		while (byteOffset < fileSize) {
			ReadBinary((char*)&chunk, this->wavData, &this->binaryPosition, sizeof(WaveChunk));
			if (strncmp(chunk.id, "fmt ", 4) == 0) {
				//fmtチャンク
				ReadBinary((char*)&fmtChunk, this->wavData, &this->binaryPosition, 16);

				//拡張fmtチャンクは読まない
				if (chunk.size > 16) {
					this->binaryPosition += chunk.size - 16;
				}
				if (fmtChunk.formatId != 1) {
					throw std::runtime_error("this waveFormat not support : " + std::string(filePass));
				}
				byteOffset += chunk.size + sizeof(WaveChunk);
				++count;
			}
			else if (strncmp(chunk.id, "data ", 4) == 0) {
				//dataチャンク
				//位置を保存して逐次読み込み用のデータに利用
				this->dataStartOffset = byteOffset + sizeof(WaveChunk);
				this->binaryPosition += chunk.size;
				byteOffset += chunk.size + sizeof(WaveChunk);
				dataSize = chunk.size;
				++count;
			}
			else {
				//それ以外のチャンクはスキップ
				this->binaryPosition += chunk.size;
				byteOffset += chunk.size + sizeof(WaveChunk);
			}
		}

		//fmt,dataのチャンク合わせて２つでない時は失敗
		if (count != 2) {
			throw std::runtime_error("illegal waveFormat (chunk has not found) : " + std::string(filePass));
		}

		this->samplingRate = fmtChunk.samplingRate;
		if (fmtChunk.numChannel == 1) {
			//モノラル
			if (fmtChunk.bitsPerSample == 8) {
				this->format = SoundFormat::Mono8;
			}
			else if (fmtChunk.bitsPerSample == 16) {
				this->format = SoundFormat::Mono16;
			}
			else {
				throw std::runtime_error("illegal waveFormat (not suppot sound channel) : " + std::string(filePass));
			}
		}
		else if(fmtChunk.numChannel == 2){
			//ステレオ
			if (fmtChunk.bitsPerSample == 8) {
				this->format = SoundFormat::Stereo8;
			}
			else if (fmtChunk.bitsPerSample == 16) {
				this->format = SoundFormat::Stereo16;
			}
			else {
				throw std::runtime_error("illegal waveFormat (not suppot sound channel) : " + std::string(filePass));
			}
		}
		else {
			throw std::runtime_error("illegal waveFormat (not suppot sound channel) : " + std::string(filePass));

		}

		this->blockSize = fmtChunk.blockSize;

		//oggと違ってこっちはループポイントが最初と最後で固定
		this->loopStart = 0;
		this->loopLength = this->loopStart + dataSize / this->blockSize;

		Seek(0);
	}

	//WaveファイルのPCMデータのシーク
	void WavData::PcmSeek(int pcmOffset) {
		//this->waveFile.seekg(this->dataStartOffset + pcmOffset * this->blockSize, std::ios_base::beg);
		this->binaryPosition = this->dataStartOffset + pcmOffset * this->blockSize;
	}

	void WavData::ReadBinary(char* dest, char* src, size_t* binaryPosition, size_t readSize) {
		//バイナリを呼んでバイナリ位置を移動
		memcpy(dest, src + (*binaryPosition), readSize);
		(*binaryPosition) += readSize;
	}

}
