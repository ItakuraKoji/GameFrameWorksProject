#include"OggData.h"
#include<fstream>

namespace K_Audio {
	////////
	//public
	////

	OggData::OggData(const char* filePass) {
		try{
			this->binaryData.oggData = nullptr;
			CreateOggFile(filePass);
			LoadFile();
		}
		catch (std::exception& e) {
			throw e;
		}
	}
	OggData::OggData(char* binaryData, size_t binarySize) {
		try {
			this->binaryData.oggData = nullptr;
			CreateOggFile(binaryData);
			LoadFile();
		}
		catch (std::exception& e) {
			throw e;
		}
	}

	OggData::~OggData() {
		ov_clear(&this->oggFile);
		if (this->binaryData.oggData != nullptr) {
			delete[] this->binaryData.oggData;
		}
	}

	void OggData::Seek(int offset) {
		ov_pcm_seek(&this->oggFile, offset);
		this->pcmOffset = offset;
	}


	////////
	//private
	////
	void OggData::CreateOggFile(const char* filePass) {
		//ファイルを読み込んで作成
		int error = ov_fopen(filePass, &this->oggFile);

		if (error != 0) {
			//エラー詳細は未実装
			switch (error) {
			case OV_EREAD:       break;
			case OV_ENOTVORBIS:  break;
			case OV_EVERSION:    break;
			case OV_EBADHEADER:  break;
			case OV_EFAULT:      break;
			default:             break;
			}
			throw std::runtime_error("fileOpen failed : " + std::string(filePass));
		}
	}

	void OggData::CreateOggFile(char* binaryData, size_t binarySize) {
		//生のデータから直接作成
		this->binaryData.classPtr = this;
		this->binaryData.oggData = binaryData;
		this->binaryData.binaryPosition = 0;
		this->binaryData.dataSize = binarySize;

		// コールバック登録
		ov_callbacks callbacks = {
			&ReadCallBack,
			&SeekCallBack,
			&CloseCallBack,
			&TellCallBack,
		};

		// Oggオープン
		if (ov_open_callbacks(&this->binaryData, &this->oggFile, 0, 0, callbacks) != 0) {
			throw std::runtime_error("oggFile create with binary failed");
		}
	}

	void OggData::LoadFile() {
		vorbis_comment* tag = ov_comment(&this->oggFile, -1);
		this->loopStart = OggCommentValue(tag, "LOOPSTART");
		this->loopLength = OggCommentValue(tag, "LOOPLENGTH");

		//ループ情報がコメントにない時は初めと最後をループポイントに
		if (!this->loopLength) {
			this->loopLength = (int)ov_pcm_total(&this->oggFile, -1);
		}

		vorbis_info* info;
		info = ov_info(&this->oggFile, -1);

		if (info->channels == 1) {
			this->format = SoundFormat::Mono16;
			this->blockSize = 2;
		}
		else {
			this->format = SoundFormat::Stereo16;
			this->blockSize = 4;
		}
		this->samplingRate = info->rate;
		this->pcmOffset = 0;
	}

	int OggData::Read(char* buffer, int maxSize) {
		int bitStream;
		//0, 2, 1の意味はそれぞれ「リトルエンディアンを使用」「WORDのサイズは16bit」「PCM符号付きを使用」
		int readSize = ov_read(&this->oggFile, buffer, maxSize, 0, 2, 1, &bitStream);
		this->pcmOffset += readSize / this->blockSize;
		return readSize;
	}

	//特定のユーザーコメントを検索し紐づいているPCM位置を返す
	int OggData::OggCommentValue(vorbis_comment* comment, const char* key) {
		int loopStart = 0;
		for (int i = 0; i < comment->comments; ++i) {
			std::string str(comment->user_comments[i]);
			std::string keyword(key);

			//keywordとstrとの部分一致を判定
			if (str.find(keyword) != std::string::npos) {
				int start = (int)str.find("=") + 1;
				int end = (int)str.length();
				//コメントに付属しているPCMの位置を取得
				return std::stoi(str.substr(start, end - start));
			}
		}
		//見つからないときは 0 を返す
		return 0;
	}

	//メモリ上のOggを扱うコールバック
	size_t OggData::ReadCallBack(void* buffer, size_t size, size_t maxCount, void* stream) {
		if (buffer == 0) {
			return 0;
		}

		//ストリームからオブジェクトのポインタを取得
		OggData::OggBinaryData* p = (OggData::OggBinaryData*)stream;

		// 取得可能カウント数を算出
		int resSize = p->dataSize - p->binaryPosition;
		size_t count = resSize / size;
		if (count > maxCount) {
			count = maxCount;
		}
		//データをコピー
		memcpy(buffer, p->oggData + p->binaryPosition, size * count);

		// ポインタ位置を移動
		p->binaryPosition += size * count;

		return count;
	}

	int OggData::SeekCallBack(void* buffer, ogg_int64_t offset, int flag) {
		//オブジェクトのポインタを取得
		OggData::OggBinaryData* p = (OggData::OggBinaryData*)buffer;

		switch (flag) {
		case SEEK_CUR:
			p->binaryPosition += offset;
			break;

		case SEEK_END:
			p->binaryPosition = p->dataSize + offset;
			break;

		case SEEK_SET:
			p->binaryPosition = offset;
			break;

		default:
			return -1;
		}

		if (p->binaryPosition > p->dataSize) {
			p->binaryPosition = p->dataSize;
			return -1;
		}
		else if (p->binaryPosition < 0) {
			p->binaryPosition = 0;
			return -1;
		}

		return 0;
	}

	int OggData::CloseCallBack(void *datasource) {
		return 0;
	}

	long OggData::TellCallBack(void *datasource) {
		return ((OggData::OggBinaryData*)datasource)->binaryPosition;
	}












}
