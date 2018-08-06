#include "GlWidget.h"

GlWidget::GlWidget(/*QSplashScreen *splash, uint splashDelayMs, */QWidget* parent) :
	QGLWidget(parent),
	m_Width(1),
	m_Height(1),
	m_ElapsedTime(0UL),
	m_FrameCounter(0),
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
	m_IlluminationUniformMVPShadow(-1),
	m_IlluminationUniformDiffuseTexture(-1),
	m_IlluminationUniformShadowTexture(-1),
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
	m_Elapsed.start();
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
#ifdef Q_OS_WIN
	glCreateShader             = reinterpret_cast<TCreateShader>            (wglGetProcAddress("glCreateShader"            ));
	glShaderSource             = reinterpret_cast<TShaderSource>            (wglGetProcAddress("glShaderSource"            ));
	glCompileShader            = reinterpret_cast<TCompileShader>           (wglGetProcAddress("glCompileShader"           ));
	glGetShaderiv              = reinterpret_cast<TGetShaderiv>             (wglGetProcAddress("glGetShaderiv"             ));
	glGetShaderInfoLog         = reinterpret_cast<TGetShaderInfoLog>        (wglGetProcAddress("glGetShaderInfoLog"        ));
	glCreateProgram            = reinterpret_cast<TCreateProgram>           (wglGetProcAddress("glCreateProgram"           ));
	glAttachShader             = reinterpret_cast<TAttachShader>            (wglGetProcAddress("glAttachShader"            ));
	glLinkProgram              = reinterpret_cast<TLinkProgram>             (wglGetProcAddress("glLinkProgram"             ));
	glGetProgramiv             = reinterpret_cast<TGetProgramiv>            (wglGetProcAddress("glGetProgramiv"            ));
	glGetProgramInfoLog        = reinterpret_cast<TGetProgramInfoLog>       (wglGetProcAddress("glGetProgramInfoLog"       ));
	glUseProgram               = reinterpret_cast<TUseProgram>              (wglGetProcAddress("glUseProgram"              ));
	glGenBuffers               = reinterpret_cast<TGenBuffers>              (wglGetProcAddress("glGenBuffers"              ));
	glBindBuffer               = reinterpret_cast<TBindBuffer>              (wglGetProcAddress("glBindBuffer"              ));
	glBufferData               = reinterpret_cast<TBufferData>              (wglGetProcAddress("glBufferData"              ));
	glActiveTexture            = reinterpret_cast<TActiveTexture>           (wglGetProcAddress("glActiveTexture"           ));
	glGenerateMipmap           = reinterpret_cast<TGenerateMipmap>          (wglGetProcAddress("glGenerateMipmap"          ));
	glGenFramebuffers          = reinterpret_cast<TGenFramebuffers>         (wglGetProcAddress("glGenFramebuffers"         ));
	glBindFramebuffer          = reinterpret_cast<TBindFramebuffer>         (wglGetProcAddress("glBindFramebuffer"         ));
	glFramebufferTexture2D     = reinterpret_cast<TFramebufferTexture2D>    (wglGetProcAddress("glFramebufferTexture2D"    ));
	glFramebufferRenderbuffer  = reinterpret_cast<TFramebufferRenderbuffer> (wglGetProcAddress("glFramebufferRenderbuffer" ));
	glGenRenderbuffers         = reinterpret_cast<TGenRenderbuffers>        (wglGetProcAddress("glGenRenderbuffers"        ));
	glBindRenderbuffer         = reinterpret_cast<TBindRenderbuffer>        (wglGetProcAddress("glBindRenderbuffer"        ));
	glRenderbufferStorage      = reinterpret_cast<TRenderbufferStorage>     (wglGetProcAddress("glRenderbufferStorage"     ));
	glCheckFramebufferStatus   = reinterpret_cast<TCheckFramebufferStatus>  (wglGetProcAddress("glCheckFramebufferStatus"  ));
	glEnableVertexAttribArray  = reinterpret_cast<TEnableVertexAttribArray> (wglGetProcAddress("glEnableVertexAttribArray" ));
	glDisableVertexAttribArray = reinterpret_cast<TDisableVertexAttribArray>(wglGetProcAddress("glDisableVertexAttribArray"));
	glVertexAttribPointer      = reinterpret_cast<TVertexAttribPointer>     (wglGetProcAddress("glVertexAttribPointer"     ));
	glGetAttribLocation        = reinterpret_cast<TGetAttribLocation>       (wglGetProcAddress("glGetAttribLocation"       ));
	glGetUniformLocation       = reinterpret_cast<TGetUniformLocation>      (wglGetProcAddress("glGetUniformLocation"      ));
	glUniform1i                = reinterpret_cast<TUniform1i>               (wglGetProcAddress("glUniform1i"               ));
	glUniform2f                = reinterpret_cast<TUniform2f>               (wglGetProcAddress("glUniform2f"               ));
	glUniform3f                = reinterpret_cast<TUniform3f>               (wglGetProcAddress("glUniform3f"               ));
	glUniform4f                = reinterpret_cast<TUniform4f>               (wglGetProcAddress("glUniform4f"               ));
	glUniformMatrix3fv         = reinterpret_cast<TUniformMatrix3fv>        (wglGetProcAddress("glUniformMatrix3fv"        ));
	glUniformMatrix4fv         = reinterpret_cast<TUniformMatrix4fv>        (wglGetProcAddress("glUniformMatrix4fv"        ));
	glDeleteBuffers            = reinterpret_cast<TDeleteBuffers>           (wglGetProcAddress("glDeleteBuffers"           ));
	glDeleteProgram            = reinterpret_cast<TDeleteProgram>           (wglGetProcAddress("glDeleteProgram"           ));
	glDeleteShader             = reinterpret_cast<TDeleteShader>            (wglGetProcAddress("glDeleteShader"            ));
	glDeleteRenderbuffers      = reinterpret_cast<TDeleteRenderbuffers>     (wglGetProcAddress("glDeleteRenderbuffers"     ));
	glDeleteFramebuffers       = reinterpret_cast<TDeleteFramebuffers>      (wglGetProcAddress("glDeleteFramebuffers"      ));
#endif

	m_IlluminationVertexShader          = CreateShader(GlConstants::ILLUMINATION_VERTEX_SHADER, GL_VERTEX_SHADER);
	m_IlluminationFragmentShader        = CreateShader(GlConstants::ILLUMINATION_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	m_IlluminationShaderProgram         = CreateProgram(m_IlluminationVertexShader, m_IlluminationFragmentShader);

	m_IlluminationAttributePosition     = glGetAttribLocation(m_IlluminationShaderProgram, "_position");
	m_IlluminationAttributeNormal       = glGetAttribLocation(m_IlluminationShaderProgram, "_normal");
	m_IlluminationAttributeTextureCoord = glGetAttribLocation(m_IlluminationShaderProgram, "_textureCoord");

	m_IlluminationUniformMVP            = glGetUniformLocation(m_IlluminationShaderProgram, "mvp");
	m_IlluminationUniformMW             = glGetUniformLocation(m_IlluminationShaderProgram, "mw");
	m_IlluminationUniformMNIT           = glGetUniformLocation(m_IlluminationShaderProgram, "mnit");
	m_IlluminationUniformMVPShadow      = glGetUniformLocation(m_IlluminationShaderProgram, "mvpShadow");

	m_IlluminationUniformDiffuseTexture = glGetUniformLocation(m_IlluminationShaderProgram, "diffTex");
	m_IlluminationUniformShadowTexture  = glGetUniformLocation(m_IlluminationShaderProgram, "shadTex");
	m_IlluminationUniformCamera         = glGetUniformLocation(m_IlluminationShaderProgram, "cam");
	m_IlluminationUniformColor          = glGetUniformLocation(m_IlluminationShaderProgram, "color");
	m_IlluminationUniformTiling         = glGetUniformLocation(m_IlluminationShaderProgram, "tiling");

	m_DepthVertexShader                 = CreateShader(GlConstants::DEPTH_VERTEX_SHADER, GL_VERTEX_SHADER);
	m_DepthFragmentShader               = CreateShader(GlConstants::DEPTH_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
	m_DepthShaderProgram                = CreateProgram(m_DepthVertexShader, m_DepthFragmentShader);

	m_DepthAttributePosition            = glGetAttribLocation(m_DepthShaderProgram, "_position");

	m_DepthUniformMVP                   = glGetUniformLocation(m_DepthShaderProgram, "mvp");

	m_CubeVertices   = CreateVertices(GlConstants::CUBE_VERTICES_COUNT,   GlConstants::CUBE_VERTICES);
	m_CubeIndices    = CreateIndices(GlConstants::CUBE_TRIANGLES_COUNT,   GlConstants::CUBE_INDICES);
	m_SphereVertices = CreateVertices(GlConstants::SPHERE_VERTICES_COUNT, GlConstants::SPHERE_VERTICES);
	m_SphereIndices  = CreateIndices(GlConstants::SPHERE_TRIANGLES_COUNT, GlConstants::SPHERE_INDICES);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_CameraView.setToIdentity();
	m_CameraView.rotate(40.0f, 1.0f, 0.0f, 0.0f);
	m_CameraView.rotate(10.0f, 0.0f, 1.0f, 0.0f);
	m_CameraView.translate(-m_CameraPosition);

	QMatrix4x4 shadowProjection;
	QMatrix4x4 shadowView;
	QMatrix4x4 shadowBias
	(
		0.5f, 0.0f, 0.0f, 0.5f,
		0.0f, 0.5f, 0.0f, 0.5f,
		0.0f, 0.0f, 0.5f, 0.5f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	shadowProjection.perspective(160.0f, 1.0f, 1.0f, 11.0f);
	//shadowProjection.ortho(-25.0f, 25.0f, -25.0f, 25.0f, 0.0f, 50.0f);
	shadowView.lookAt(QVector3D(-7.0f, 10.0f, -10.0f), QVector3D(-7.0f, 0.0f, -10.0f), QVector3D(0.0f, 0.0f, -1.0f));

	m_CameraShadowViewProjetion.setToIdentity();
	m_CameraShadowViewProjetionBias.setToIdentity();

	m_CameraShadowViewProjetion     = shadowProjection * shadowView;
	m_CameraShadowViewProjetionBias = shadowBias * m_CameraShadowViewProjetion;

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

	glGenFramebuffers(1, &m_ShadowFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebuffer);

	glGenTextures(1, &m_ShadowTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SHADOW_TEXTURE_WIDTH, SHADOW_TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ShadowTexture, 0);

	glGenRenderbuffers(1, &m_ShadowDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, m_ShadowDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, SHADOW_TEXTURE_WIDTH, SHADOW_TEXTURE_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_ShadowDepth);

	GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		qDebug() << "Framebuffer creation error:" << fboStatus;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	emit Initialized();

	if(m_Scenes.size() > m_ActiveSceneIdx && m_Scenes[m_ActiveSceneIdx] != NULL)
	{
		m_Scenes[m_ActiveSceneIdx]->Initialize();
	}
}

void GlWidget::resizeGL(int w, int h)
{
	m_Width  = w;
	m_Height = h;

	glViewport(0, 0, m_Width, m_Height);

	m_CameraProjetion.setToIdentity();
	m_CameraProjetion.perspective(60.0f, (float)w / (float)h, 0.1f, 100.0f);
}

void GlWidget::paintGL()
{
	uint64_t timeStart = m_Elapsed.elapsed();

	glViewport(0, 0, SHADOW_TEXTURE_WIDTH, SHADOW_TEXTURE_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ShadowFramebuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_DepthShaderProgram);

	glEnable(GL_POLYGON_OFFSET_FILL);
#ifdef Q_OS_SYMBIAN
	glPolygonOffset(1.0f, 1.0f);
#else
	glPolygonOffset(1.5f, 10.0f);
#endif

	GLuint lastVertices = 0;
	GLuint lastIndices  = 0;

	for(int idx = 0, count = m_RenderObjects.size(); idx < count; idx++)
	{
		RenderObject* object = &m_RenderObjects[idx];

		if(lastVertices != object->Vertices || lastIndices != object->Indices)
		{
			if(idx > 0)
			{
				glDisableVertexAttribArray(m_DepthAttributePosition);
			}

			glBindBuffer(GL_ARRAY_BUFFER, object->Vertices);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->Indices);

			glEnableVertexAttribArray(m_DepthAttributePosition);
			glVertexAttribPointer(m_DepthAttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 0));
		}

		object->ModelWorld = object->SceneObject->GetTransform();
		QMatrix4x4 modelViewProjection = m_CameraShadowViewProjetion * object->ModelWorld;

		glUniformMatrix4fv(m_DepthUniformMVP, 1, GL_FALSE, Matrix4x4ToFloat(modelViewProjection).data());

		glDrawElements(GL_TRIANGLES, object->Faces * 3, GL_UNSIGNED_SHORT, NULL);

		lastVertices = object->Vertices;
		lastIndices  = object->Indices;
	}

	if(m_RenderObjects.size() > 0)
	{
		glDisableVertexAttribArray(m_DepthAttributePosition);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glPolygonOffset(0.0f, 0.0f);
	glDisable(GL_POLYGON_OFFSET_FILL);

	glViewport(0, 0, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_IlluminationShaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GridTexture);
	glUniform1i(m_IlluminationUniformDiffuseTexture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
	glUniform1i(m_IlluminationUniformShadowTexture, 1);

	QMatrix4x4 cameraViewProjetion = m_CameraProjetion * m_CameraView;

	glUniform3f(m_IlluminationUniformCamera, m_CameraPosition.x(), m_CameraPosition.y(), m_CameraPosition.z());

	lastVertices = 0;
	lastIndices  = 0;

	for(int idx = 0, count = m_RenderObjects.size(); idx < count; idx++)
	{
		const RenderObject* object = &m_RenderObjects[idx];

		if(lastVertices != object->Vertices || lastIndices != object->Indices)
		{
			if(idx > 0)
			{
				glDisableVertexAttribArray(m_IlluminationAttributePosition);
				glDisableVertexAttribArray(m_IlluminationAttributeNormal);
				glDisableVertexAttribArray(m_IlluminationAttributeTextureCoord);
			}

			glBindBuffer(GL_ARRAY_BUFFER, object->Vertices);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->Indices);

			glEnableVertexAttribArray(m_IlluminationAttributePosition);
			glEnableVertexAttribArray(m_IlluminationAttributeNormal);
			glEnableVertexAttribArray(m_IlluminationAttributeTextureCoord);
			glVertexAttribPointer(m_IlluminationAttributePosition,     3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 0));
			glVertexAttribPointer(m_IlluminationAttributeNormal,       3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 3));
			glVertexAttribPointer(m_IlluminationAttributeTextureCoord, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, reinterpret_cast<float *>(sizeof(float) * 6));
		}

		QMatrix4x4 modelViewProjection                = cameraViewProjetion * object->ModelWorld;
		QMatrix3x3 modelWorldNormalInversedTransposed = object->ModelWorld.inverted().transposed().normalMatrix();
		QMatrix4x4 modelViewProjectionShadow          = m_CameraShadowViewProjetionBias * object->ModelWorld;

		glUniformMatrix4fv(m_IlluminationUniformMVP,       1, GL_FALSE, Matrix4x4ToFloat(modelViewProjection).data());
		glUniformMatrix4fv(m_IlluminationUniformMW,        1, GL_FALSE, Matrix4x4ToFloat(object->ModelWorld).data());
		glUniformMatrix3fv(m_IlluminationUniformMNIT,      1, GL_FALSE, Matrix3x3ToFloat(modelWorldNormalInversedTransposed).data());
		glUniformMatrix4fv(m_IlluminationUniformMVPShadow, 1, GL_FALSE, Matrix4x4ToFloat(modelViewProjectionShadow).data());

		glUniform4f(m_IlluminationUniformColor, object->Color.x(), object->Color.y(), object->Color.z(), object->Color.w());
		glUniform2f(m_IlluminationUniformTiling, object->Tiling.x(), object->Tiling.y());

		glDrawElements(GL_TRIANGLES, object->Faces * 3, GL_UNSIGNED_SHORT, NULL);

		lastVertices = object->Vertices;
		lastIndices  = object->Indices;
	}

	if(m_RenderObjects.size() > 0)
	{
		glDisableVertexAttribArray(m_IlluminationAttributePosition);
		glDisableVertexAttribArray(m_IlluminationAttributeNormal);
		glDisableVertexAttribArray(m_IlluminationAttributeTextureCoord);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgram(0);

	uint64_t timeEnd   = m_Elapsed.elapsed();
	uint64_t deltaTime = timeEnd - timeStart;
	m_ElapsedTime += deltaTime;
	m_FrameCounter++;

	emit StatsUpdated_RenderSec(deltaTime * 0.001f);

	if(m_ElapsedTime > 1000)
	{
		emit StatsUpdated_RenderMs(m_ElapsedTime / m_FrameCounter);

		m_ElapsedTime  = 0UL;
		m_FrameCounter = 0;
	}
}

void GlWidget::AddBox(const ISceneObjectProvider* sceneObject, const QVector4D& color, const QVector2D& tiling)
{
	m_RenderObjects.push_back(RenderObject
	{
		m_CubeVertices,
		m_CubeIndices,
		GlConstants::CUBE_TRIANGLES_COUNT,
		color,
		tiling,
		QMatrix4x4(),
		sceneObject
	});
}

void GlWidget::AddSpere(const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling)
{
	m_RenderObjects.push_back(RenderObject
	{
		m_SphereVertices,
		m_SphereIndices,
		GlConstants::SPHERE_TRIANGLES_COUNT,
		color,
		tiling,
		QMatrix4x4(),
		sceneObject
	});
}

void GlWidget::AddMesh(const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling)
{
	QVector<float>  meshVertices = sceneObject->GetVertices();
	QVector<ushort> meshIndices  = sceneObject->GetIndices();
	uint   indicesCount = meshIndices.size() / 3;

	GLuint vertexBuffer = CreateVertices(meshVertices.size(), meshVertices.constData());
	GLuint indexBuffer  = CreateIndices(indicesCount, meshIndices.constData());

	m_RenderObjects.push_back(RenderObject
	{
		vertexBuffer,
		indexBuffer,
		indicesCount,
		color,
		tiling,
		QMatrix4x4(),
		sceneObject
	});
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

	emit StatsUpdated_SceneIdx(m_ActiveSceneIdx);
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

	emit StatsUpdated_SceneIdx(m_ActiveSceneIdx);
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

GLuint GlWidget::CreateVertexBuffer(GLenum type, uint size, const void* data)
{
	GLuint vertexBuffer;

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(type, vertexBuffer);
	glBufferData(type, size, data, GL_STATIC_DRAW);
	glBindBuffer(type, 0);

	m_VertexBuffers.push_back(vertexBuffer);

	return vertexBuffer;
}

GLuint GlWidget::CreateVertices(uint count, const void* data)
{
	return CreateVertexBuffer(GL_ARRAY_BUFFER, sizeof(float) * 8 * count, data);
}

GLuint GlWidget::CreateIndices(uint count, const void* data)
{
	return CreateVertexBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * 3 * count, data);
}
