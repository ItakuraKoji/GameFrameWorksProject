#include"ModelDataFactory.h"

namespace K_Graphics {
	////////
	//public
	////

	void ModelDataFactory::CreateK3MDModelFromFBX(const std::string& fileName, TextureList* textureList) {
		K_Loader::FbxModelLoader loader;
		if (!loader.LoadFBX(fileName, textureList)) {
			throw std::runtime_error("FBX Load Failed : " + fileName);
		}
		K_Loader::K3MDLoader k3mdLoader;
		k3mdLoader.CreateBinaryFile(loader.PassModelData(), (fileName + ".k3md").data());
	}


	ModelDatas* ModelDataFactory::LoadFBXModel(const std::string& fileName, TextureList* textureList) {
		K_Loader::FbxModelLoader loader;
		if (!loader.LoadFBX(fileName, textureList)) {
			throw std::runtime_error("FBX Load Failed : " + fileName);
		}
		//�f�R�[�h�ς݂̃f�[�^��ǂݍ���
		K_Loader::K3MDLoader k3mdLoader;
		ModelResource* resource = k3mdLoader.LoadModel(loader.PassModelData(), textureList);

		ModelDatas* data = new ModelDatas;

		//�A�j���[�V�������͑��݂��Ȃ��ꍇ������iNULL�j
		data->vertexBuffer = resource->vertexBuffer;
		data->material = resource->material;
		data->bone = resource->bone;
		if (data->bone != nullptr) {
			data->animation = new AnimationData(data->bone, data->vertexBuffer->GetNumBuffer());
		}
		else {
			data->animation = nullptr;
		}

		//ModelResource�͂����K�v�Ȃ��̂ŁA�v�f��null�ɂ��ĉ��(�f�X�g���N�^�ŏ�����Ȃ��悤�ɂ��邽��)
		resource->vertexBuffer = nullptr;
		resource->bone = nullptr;
		resource->material = nullptr;
		delete resource;

		return data;
	}

	ModelDatas* ModelDataFactory::LoadK_3DModel(const std::string& fileName, TextureList* textureList) {
		//���ݒ��A�Ǝ��́u.k3md�v�`���̃��f���f�[�^��ǂݍ��ނ���
		K_Loader::K3MDLoader loader;
		ModelResource* resource = loader.LoadModel(fileName.data(), textureList);

		ModelDatas* data = new ModelDatas;
		//�A�j���[�V�������͑��݂��Ȃ��ꍇ������iNULL�j
		data->vertexBuffer = resource->vertexBuffer;
		data->material = resource->material;
		data->bone = resource->bone;
		if (data->bone != nullptr) {
			data->animation = new AnimationData(data->bone, data->vertexBuffer->GetNumBuffer());
		}
		else {
			data->animation = nullptr;
		}

		//ModelResource�͂����K�v�Ȃ��̂ŁA�v�f��null�ɂ��ĉ��(�f�X�g���N�^�ŏ�����Ȃ��悤�ɂ��邽��)
		resource->vertexBuffer = nullptr;
		resource->bone = nullptr;
		resource->material = nullptr;
		delete resource;

		return data;
	}

	//!���f���f�[�^�̃��\�[�X�쐬�i�Ԃ��|�C���^�̊J���ӔC������j
	ModelResource* ModelDataFactory::CreateModelResourceFromFBX(const std::string& fileName, TextureList* textureList) {
		K_Loader::FbxModelLoader loader;
		if (!loader.LoadFBX(fileName, textureList)) {
			throw std::runtime_error("FBX Load Failed : " + fileName);
		}

		K_Loader::K3MDLoader k3mdLoader;
		ModelResource* resource = k3mdLoader.LoadModel(loader.PassModelData(), textureList);
		return resource;
	}
	ModelResource* ModelDataFactory::CreateModelResourceFromK3MD(const std::string& fileName, TextureList* textureList) {
		K_Loader::K3MDLoader k3mdLoader;
		ModelResource* resource = k3mdLoader.LoadModel(fileName.data(), textureList);
		return resource;
	}


	//!���\�[�X����f�[�^���؂�ă��f�����쐬�i�Ԃ��|�C���^�̊J���ӔC������j
	ModelDatasCopy* ModelDataFactory::CreateModelDataCopy(ModelResource* resource) {
		ModelDatasCopy* data = new ModelDatasCopy;

		//�A�j���[�V�������͑��݂��Ȃ��ꍇ������iNULL�j
		data->vertexBuffer = resource->vertexBuffer;
		data->material = resource->material;
		data->bone = resource->bone;
		if (data->bone) {
			data->animation = new AnimationData(data->bone, data->vertexBuffer->GetNumBuffer());
		}
		else {
			data->animation = nullptr;
		}
		return data;
	}

	ModelDatas* ModelDataFactory::CreateSquareModel(float width, float height, Texture* texture, bool isCenter) {
		struct Vertex {
			K_Math::Vector3 pos;
			K_Math::Vector2 uv;
		};

		ModelDatas* data = new ModelDatas;
		data->vertexBuffer = new VertexData;
		data->material = new MaterialData;

		Vertex vertex[4];
		if (isCenter) {
			float right = width * 0.5f;
			float left = -width * 0.5f;
			float up = height * 0.5f;
			float down = -height * 0.5f;
			vertex[0].pos = K_Math::Vector3(left, down, 0.0f);
			vertex[1].pos = K_Math::Vector3(right, down, 0.0f);
			vertex[2].pos = K_Math::Vector3(right, up, 0.0f);
			vertex[3].pos = K_Math::Vector3(left, up, 0.0f);
		}
		else {
			vertex[0].pos = K_Math::Vector3(0.0f, -height, 0.0f);
			vertex[1].pos = K_Math::Vector3(width, -height, 0.0f);
			vertex[2].pos = K_Math::Vector3(width, 0.0f, 0.0f);
			vertex[3].pos = K_Math::Vector3(0.0f, 0.0f, 0.0f);
		}

		vertex[0].uv = K_Math::Vector2(0.0f, 0.0f);
		vertex[1].uv = K_Math::Vector2(1.0f, 0.0f);
		vertex[2].uv = K_Math::Vector2(1.0f, 1.0f);
		vertex[3].uv = K_Math::Vector2(0.0f, 1.0f);

		unsigned int index[6];
		index[0] = 0;
		index[1] = 2;
		index[2] = 3;
		index[3] = 0;
		index[4] = 1;
		index[5] = 2;

		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertex, GL_STATIC_DRAW);

		//position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		//uv
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (unsigned char*)NULL + 3 * sizeof(float));



		std::vector<GLuint> IBOs;
		IBOs.resize(1);
		glGenBuffers(1, &IBOs[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOs[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), index, GL_STATIC_DRAW);

		VertexBuffer vertexBuffer;
		vertexBuffer.VAO = VAO;
		vertexBuffer.VBO = VBO;
		vertexBuffer.IBOs = IBOs;
		vertexBuffer.numFace = 2;
		vertexBuffer.numMaterial = 1;

		glBindVertexArray(0);

		data->vertexBuffer->Add(vertexBuffer);

		//���b�V���z��͈�����ŁA�}�e���A�������̒��Ɉ���������Ă���
		std::vector<Material> material;
		material.resize(1);

		material[0].ambient = K_Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		material[0].ambientPower = 0.0f;
		material[0].diffuse = K_Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
		material[0].specular = K_Math::Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		material[0].specurarShininess = 0.0f;
		material[0].specurarPower = 0.0f;
		material[0].numFace = 6;

		//material[0].textureName = textureName;
		material[0].texture = texture;

		data->material->Add(material);
		return data;
	}
}

