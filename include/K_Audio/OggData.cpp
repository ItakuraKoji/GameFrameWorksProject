#include"OggData.h"

namespace K_Audio {

	////////
	//public
	////

	OggData::OggData(const char* filePass) {
		try{
			LoadFile(filePass);
		}
		catch (std::exception& e) {
			throw e;
		}
	}

	OggData::~OggData() {
		ov_clear(&this->oggFile);
	}

	void OggData::Seek(int offset) {
		ov_pcm_seek(&this->oggFile, offset);
		this->pcmOffset = offset;
	}


	////////
	//private
	////

	void OggData::LoadFile(const char* filePass) {
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

}
