#include "GlWidget.h"

GlWidget::GlWidget(QSplashScreen *splash, uint splashDelayMs, QWidget* parent) :
	QGLWidget(parent),
	m_Splash(splash),
	m_PrevSceneButton(new QPushButton("«", this)),
	m_NextSceneButton(new QPushButton("»", this)),
	m_ActiveSceneIdx(0),
	m_CameraPosition(-1.0f, 8.5f, 9.0f),
	m_VertexShader(0),
	m_FragmentShader(0),
	m_ShaderProgram(0),
	m_AttributePosition(-1),
	m_AttributeNormal(-1),
	m_AttributeTextureCoord(-1),
	m_UniformMVP(-1),
	m_UniformMW(-1),
	m_UniformMNIT(-1),
	m_UniformDiffuseTexture(-1),
	m_UniformCamera(-1),
	m_UniformColor(-1),
	m_UniformTiling(-1),
	m_CubeVertices(0),
	m_CubeIndices(0),
	m_SphereVertices(0),
	m_SphereIndices(0),
	m_GridTexture(0)
{
	setAttribute(Qt::WA_LockLandscapeOrientation);

	connect(m_PrevSceneButton, SIGNAL(clicked()), this, SLOT(ActivatePrevScene()));
	connect(m_NextSceneButton, SIGNAL(clicked()), this, SLOT(ActivateNextScene()));

	QTimer::singleShot(splashDelayMs, this, SLOT(Initialize()));
}

GlWidget::~GlWidget()
{
	foreach(vertexBuffer, m_VertexBuffers)
	{
		glDeleteBuffers(1, &(*vertexBuffer));
	}

	glDeleteProgram(m_ShaderProgram);
	glDeleteShader(m_VertexShader);
	glDeleteShader(m_FragmentShader);

	glDeleteTextures(1, &m_GridTexture);
}

void GlWidget::AddScene(ISceneProvider *scene)
{
	m_Scenes.push_back(scene);
}

void GlWidget::initializeGL()
{
	m_VertexShader   = CreateShader(GlConstants::VERTEX_SHADER, GL_VERTEX_SHADER);
	m_FragmentShader = CreateShader(GlConstants::FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	m_ShaderProgram  = CreateProgram(m_VertexShader, m_FragmentShader);

	m_AttributePosition     = glGetAttribLocation(m_ShaderProgram, "_position");
	m_AttributeNormal       = glGetAttribLocation(m_ShaderProgram, "_normal");
	m_AttributeTextureCoord = glGetAttribLocation(m_ShaderProgram, "_textureCoord");

	m_UniformMVP            = glGetUniformLocation(m_ShaderProgram, "mvp");
	m_UniformMW             = glGetUniformLocation(m_ShaderProgram, "mw");
	m_UniformMNIT           = glGetUniformLocation(m_ShaderProgram, "mnit");

	m_UniformDiffuseTexture = glGetUniformLocation(m_ShaderProgram, "diffTex");
	m_UniformCamera         = glGetUniformLocation(m_ShaderProgram, "cam");
	m_UniformColor          = glGetUniformLocation(m_ShaderProgram, "color");
	m_UniformTiling         = glGetUniformLocation(m_ShaderProgram, "tiling");

	CreateVertices(&m_CubeVertices,   GlConstants::CUBE_VERTICES_COUNT,   GlConstants::CUBE_VERTICES);
	CreateIndices (&m_CubeIndices,    GlConstants::CUBE_INDICES_COUNT,    GlConstants::CUBE_INDICES);
	CreateVertices(&m_SphereVertices, GlConstants::SPHERE_VERTICES_COUNT, GlConstants::SPHERE_VERTICES);
	CreateIndices (&m_SphereIndices,  GlConstants::SPHERE_INDICES_COUNT,  GlConstants::SPHERE_INDICES);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_CameraView.setToIdentity();
	m_CameraView.rotate(40.0f, 1.0f, 0.0f, 0.0f);
	m_CameraView.rotate(10.0f, 0.0f, 1.0f, 0.0f);
	m_CameraView.translate(-m_CameraPosition);

	glGenTextures(1, &m_GridTexture);
	glBindTexture(GL_TEXTURE_2D, m_GridTexture);

	QImage gridImage(":/images/uv.png");
	gridImage = gridImage.convertToFormat(QImage::Format_RGB32);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gridImage.width(), gridImage.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, gridImage.constBits());

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	if(m_Splash != NULL)
	{
		m_Splash->close();
	}

	emit Initialized();

	if(m_Scenes.size() > m_ActiveSceneIdx && m_Scenes[m_ActiveSceneIdx] != NULL)
	{
		m_Scenes[m_ActiveSceneIdx]->Initialize();
	}
}

void GlWidget::resizeGL(int w, int h)
{
	m_PrevSceneButton->setGeometry(           0, h - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);
	m_NextSceneButton->setGeometry(BUTTON_WIDTH, h - BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT);

	glViewport(0, 0, w, h);

	m_CameraProjetion.setToIdentity();
	m_CameraProjetion.perspective(60.0f, (float)w / (float)h, 0.1f, 100.0f);
}

void GlWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_ShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GridTexture);
	glUniform1i(m_UniformDiffuseTexture, 0);

	for(int idx = 0, count = m_RenderObjects.size(); idx < count; idx++)
	{
		const RenderObject* object = &m_RenderObjects[idx];

		glBindBuffer(GL_ARRAY_BUFFER, object->Vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->Indices);

		glEnableVertexAttribArray(m_AttributePosition);
		glEnableVertexAttribArray(m_AttributeNormal);
		glEnableVertexAttribArray(m_AttributeTextureCoord);
		glVertexAttribPointer(m_AttributePosition,     3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 0));
		glVertexAttribPointer(m_AttributeNormal,       3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 3));
		glVertexAttribPointer(m_AttributeTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 6));

		QMatrix4x4 modelWorld          = object->SceneObject->GetTransform();
		QMatrix4x4 modelViewProjection = m_CameraProjetion * m_CameraView * modelWorld;
		QMatrix3x3 modelWorldNormalInversedTransposed = modelWorld.inverted().transposed().normalMatrix();

		glUniformMatrix4fv(m_UniformMVP,  1, GL_FALSE, Matrix4x4ToFloat(modelViewProjection).data());
		glUniformMatrix4fv(m_UniformMW,   1, GL_FALSE, Matrix4x4ToFloat(modelWorld).data());
		glUniformMatrix3fv(m_UniformMNIT, 1, GL_FALSE, Matrix3x3ToFloat(modelWorldNormalInversedTransposed).data());

		glUniform3f(m_UniformCamera, m_CameraPosition.x(), m_CameraPosition.y(), m_CameraPosition.z());
		glUniform4f(m_UniformColor, object->Color.x(), object->Color.y(), object->Color.z(), object->Color.w());
		glUniform2f(m_UniformTiling, object->Tiling.x(), object->Tiling.y());

		glDrawElements(GL_TRIANGLES, object->Faces * 3, GL_UNSIGNED_SHORT, NULL);

		glDisableVertexAttribArray(m_AttributePosition);
		glDisableVertexAttribArray(m_AttributeNormal);
		glDisableVertexAttribArray(m_AttributeTextureCoord);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);
}

void GlWidget::AddBox(const ISceneObjectProvider* sceneObject, const QVector4D& color, const QVector2D& tiling)
{
	m_RenderObjects.push_back(RenderObject
	{
		m_CubeVertices,
		m_CubeIndices,
		GlConstants::CUBE_INDICES_COUNT,
		color,
		tiling,
		sceneObject
	});
}

void GlWidget::AddSpere(const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling)
{
	m_RenderObjects.push_back(RenderObject
	{
		m_SphereVertices,
		m_SphereIndices,
		GlConstants::SPHERE_INDICES_COUNT,
		color,
		tiling,
		sceneObject
	});
}

void GlWidget::AddMesh(const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling)
{
}

void GlWidget::Initialize()
{
	showFullScreen();
}

void GlWidget::ActivatePrevScene()
{
	m_Scenes[m_ActiveSceneIdx]->Deinitialize();
	m_RenderObjects.clear();

	m_ActiveSceneIdx--;
	if(m_ActiveSceneIdx < 0)
	{
		m_ActiveSceneIdx = m_Scenes.size() - 1;
	}

	m_Scenes[m_ActiveSceneIdx]->Initialize();
}

void GlWidget::ActivateNextScene()
{
	m_Scenes[m_ActiveSceneIdx]->Deinitialize();
	m_RenderObjects.clear();

	m_ActiveSceneIdx++;
	if(m_ActiveSceneIdx >= m_Scenes.size())
	{
		m_ActiveSceneIdx = 0;
	}

	m_Scenes[m_ActiveSceneIdx]->Initialize();
}

GLuint GlWidget::CreateShader(const char *shaderText, GLenum type)
{
	GLint status     = 0;
	GLint infoLength = 0;
	char* log;

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderText, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 0)
	{
		log = new char[infoLength + 0];
		glGetShaderInfoLog(shader, infoLength, &infoLength, log);
		log[infoLength] = 0;

		if(log[0] != 0)
		{
			qDebug() << log << "\n";
		}
	}

	return shader;
}

GLuint GlWidget::CreateProgram(GLuint vertexShader, GLuint fragmentShader)
{
	GLint status     = 0;
	GLint infoLength = 0;
	char* log;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);

	if(infoLength > 0)
	{
		log = new char[infoLength + 0];
		glGetProgramInfoLog(program, infoLength, &infoLength, log);
		log[infoLength] = 0;

		if(log[0] != 0)
		{
			qDebug() << log << "\n";
		}
	}

	return program;
}

void GlWidget::CreateVertexBuffer(GLuint *vertexBuffer, GLenum type, uint size, const void* data)
{
	glGenBuffers(1, vertexBuffer);
	glBindBuffer(type, *vertexBuffer);
	glBufferData(type, size, data, GL_STATIC_DRAW);
	glBindBuffer(type, 0);

	m_VertexBuffers.push_back(*vertexBuffer);
}

void GlWidget::CreateVertices(GLuint *vertexBuffer, uint count, const void* data)
{
	CreateVertexBuffer(vertexBuffer, GL_ARRAY_BUFFER, sizeof(float) * 8 * count, data);
}

void GlWidget::CreateIndices(GLuint *vertexBuffer, uint count, const void* data)
{
	CreateVertexBuffer(vertexBuffer, GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * 3 * count, data);
}
