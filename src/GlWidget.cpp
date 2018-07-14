#include "GlWidget.h"

GlWidget::GlWidget(QSplashScreen *splash, uint splashDelayMs, QWidget* parent) :
	QGLWidget(parent),
	m_Splash(splash),
	m_PrevSceneButton(new QPushButton("«", this)),
	m_NextSceneButton(new QPushButton("»", this)),
	m_ActiveSceneIdx(0),
	m_CameraPosition(-1.0f, 8.5f, 9.0f),
	m_IlluminationVertexShader(0),
	m_IlluminationFragmentShader(0),
	m_IlluminationShaderProgram(0),
	m_IlluminationAttributePosition(-1),
	m_IlluminationAttributeNormal(-1),
	m_IlluminationAttributeTextureCoord(-1),
	m_IlluminationUniformMVP(-1),
	m_IlluminationUniformMW(-1),
	m_IlluminationUniformMNIT(-1),
	m_IlluminationUniformDiffuseTexture(-1),
	m_IlluminationUniformCamera(-1),
	m_IlluminationUniformColor(-1),
	m_IlluminationUniformTiling(-1),
	m_DepthVertexShader(0),
	m_DepthFragmentShader(0),
	m_DepthShaderProgram(0),
	m_DepthAttributePosition(-1),
	m_DepthUniformMVP(-1),
	m_CubeVertices(0),
	m_CubeIndices(0),
	m_SphereVertices(0),
	m_SphereIndices(0),
	m_GridTexture(0),
	m_ShadowTexture(0),
	m_ShadowDepth(0)
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

	glDeleteProgram(m_IlluminationShaderProgram);
	glDeleteShader(m_IlluminationVertexShader);
	glDeleteShader(m_IlluminationFragmentShader);

	glDeleteProgram(m_DepthShaderProgram);
	glDeleteShader(m_DepthVertexShader);
	glDeleteShader(m_DepthFragmentShader);

	glDeleteTextures(1, &m_GridTexture);

	glDeleteTextures(1, &m_ShadowTexture);
	glDeleteRenderbuffers(1, &m_ShadowDepth);
	glDeleteFramebuffers(1, &m_ShadowFramebuffer);
}

void GlWidget::AddScene(ISceneProvider *scene)
{
	m_Scenes.push_back(scene);
}

void GlWidget::initializeGL()
{
	m_IlluminationVertexShader          = CreateShader(GlConstants::ILLUMINATION_VERTEX_SHADER, GL_VERTEX_SHADER);
	m_IlluminationFragmentShader        = CreateShader(GlConstants::ILLUMINATION_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	m_IlluminationShaderProgram         = CreateProgram(m_IlluminationVertexShader, m_IlluminationFragmentShader);

	m_IlluminationAttributePosition     = glGetAttribLocation(m_IlluminationShaderProgram, "_position");
	m_IlluminationAttributeNormal       = glGetAttribLocation(m_IlluminationShaderProgram, "_normal");
	m_IlluminationAttributeTextureCoord = glGetAttribLocation(m_IlluminationShaderProgram, "_textureCoord");

	m_IlluminationUniformMVP            = glGetUniformLocation(m_IlluminationShaderProgram, "mvp");
	m_IlluminationUniformMW             = glGetUniformLocation(m_IlluminationShaderProgram, "mw");
	m_IlluminationUniformMNIT           = glGetUniformLocation(m_IlluminationShaderProgram, "mnit");

	m_IlluminationUniformDiffuseTexture = glGetUniformLocation(m_IlluminationShaderProgram, "diffTex");
	m_IlluminationUniformCamera         = glGetUniformLocation(m_IlluminationShaderProgram, "cam");
	m_IlluminationUniformColor          = glGetUniformLocation(m_IlluminationShaderProgram, "color");
	m_IlluminationUniformTiling         = glGetUniformLocation(m_IlluminationShaderProgram, "tiling");

	m_DepthVertexShader                 = CreateShader(GlConstants::DEPTH_VERTEX_SHADER, GL_VERTEX_SHADER);
	m_DepthFragmentShader               = CreateShader(GlConstants::DEPTH_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	m_DepthShaderProgram                = CreateProgram(m_DepthVertexShader, m_DepthFragmentShader);

	m_DepthAttributePosition            = glGetAttribLocation(m_DepthShaderProgram, "_position");

	m_DepthUniformMVP                   = glGetUniformLocation(m_DepthShaderProgram, "mvp");

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
	//m_CameraView.lookAt(QVector3D(-7.0f, 10.0f, -10.0f), QVector3D(), QVector3D(0.0f, 1.0f, 0.0f));

	glGenTextures(1, &m_GridTexture);
	glBindTexture(GL_TEXTURE_2D, m_GridTexture);

	QImage gridImage(":/images/uv.png");
	gridImage = gridImage.convertToFormat(QImage::Format_RGB888);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gridImage.width(), gridImage.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, gridImage.constBits());

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*glGenFramebuffers(1, &m_ShadowFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebuffer);

	glGenTextures(1, &m_ShadowTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ShadowTexture, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &m_ShadowDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ShadowDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 256, 256);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_ShadowDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		qDebug() << "Framebuffer creation error:" << fboStatus;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

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
	//m_CameraProjetion.perspective(90.0f, 1.0f, 1.0f, 100.0f);
}

void GlWidget::paintGL()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_IlluminationShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GridTexture);
	glUniform1i(m_IlluminationUniformDiffuseTexture, 0);

	for(int idx = 0, count = m_RenderObjects.size(); idx < count; idx++)
	{
		const RenderObject* object = &m_RenderObjects[idx];

		glBindBuffer(GL_ARRAY_BUFFER, object->Vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->Indices);

		glEnableVertexAttribArray(m_IlluminationAttributePosition);
		glEnableVertexAttribArray(m_IlluminationAttributeNormal);
		glEnableVertexAttribArray(m_IlluminationAttributeTextureCoord);
		glVertexAttribPointer(m_IlluminationAttributePosition,     3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 0));
		glVertexAttribPointer(m_IlluminationAttributeNormal,       3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 3));
		glVertexAttribPointer(m_IlluminationAttributeTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 6));

		QMatrix4x4 modelWorld          = object->SceneObject->GetTransform();
		QMatrix4x4 modelViewProjection = m_CameraProjetion * m_CameraView * modelWorld;
		QMatrix3x3 modelWorldNormalInversedTransposed = modelWorld.inverted().transposed().normalMatrix();

		glUniformMatrix4fv(m_IlluminationUniformMVP,  1, GL_FALSE, Matrix4x4ToFloat(modelViewProjection).data());
		glUniformMatrix4fv(m_IlluminationUniformMW,   1, GL_FALSE, Matrix4x4ToFloat(modelWorld).data());
		glUniformMatrix3fv(m_IlluminationUniformMNIT, 1, GL_FALSE, Matrix3x3ToFloat(modelWorldNormalInversedTransposed).data());

		glUniform3f(m_IlluminationUniformCamera, m_CameraPosition.x(), m_CameraPosition.y(), m_CameraPosition.z());
		glUniform4f(m_IlluminationUniformColor, object->Color.x(), object->Color.y(), object->Color.z(), object->Color.w());
		glUniform2f(m_IlluminationUniformTiling, object->Tiling.x(), object->Tiling.y());

		glDrawElements(GL_TRIANGLES, object->Faces * 3, GL_UNSIGNED_SHORT, NULL);

		glDisableVertexAttribArray(m_IlluminationAttributePosition);
		glDisableVertexAttribArray(m_IlluminationAttributeNormal);
		glDisableVertexAttribArray(m_IlluminationAttributeTextureCoord);

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
