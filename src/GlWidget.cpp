#include "GlWidget.h"

GlWidget::GlWidget(QWidget* parent, QSplashScreen *splash, uint splashDelayMs) :
	QGLWidget(parent),
	m_Splash(splash),
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
	m_GridTexture(0)
{
	setAttribute(Qt::WA_LockLandscapeOrientation);
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

void GlWidget::initializeGL()
{
	const char *VERTEX_SHADER =
	{
		"attribute vec3 _position;\n"
		"attribute vec3 _normal;\n"
		"attribute vec2 _textureCoord;\n"
		"\n"
		"uniform mat4 mvp;\n"
		"uniform mat4 mw;\n"
		"uniform mat3 mnit;\n"
		"\n"
		"varying vec3 positionWorld;\n"
		"varying vec3 normalDir;\n"
		"varying vec2 texCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	positionWorld = vec4(mw * vec4(_position, 1.0)).xyz;\n"
		"	normalDir     = normalize(mnit * normalize(_normal));\n"
		"	texCoord      = _textureCoord;\n"
		"	gl_Position   = mvp * vec4(_position, 1.0);\n"
		"}\n"
	};

	const char *FRAGMENT_SHADER =
	{
		"varying vec3 positionWorld;\n"
		"varying vec3 normalDir;\n"
		"varying vec2 texCoord;\n"
		"\n"
		"uniform sampler2D diffTex;\n"
		"\n"
		"uniform vec3 cam;\n"
		"uniform vec3 color;\n"
		"uniform vec2 tiling;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	vec3  ambColor       = vec3(0.3, 0.4, 0.5);\n"
		"	vec3  lightPos       = vec3(-7.0, 10.0, -10.0);\n"
		"	vec2  lightRange     = vec2(1.0, 50.0);\n"
		"	vec3  lightColor     = vec3(1.5, 1.2, 1.0);\n"
		"	vec4  lightSpecColor = vec4(1.0, 1.0, 1.0, 16.0);\n"
		"\n"
		"	vec3  diffuse        = texture2D(diffTex, texCoord * tiling).rgb;\n"
		"	float diffuseGray    = diffuse.r * 0.3 + diffuse.g * 0.59 + diffuse.b * 0.11;\n"
		"\n"
		"	vec3  viewDir        = normalize(cam - positionWorld);\n"
		"	vec3  lightDir       = lightPos - positionWorld;\n"
		"\n"
		"	float lightDist      = clamp((length(lightDir) - lightRange.x) / (lightRange.y - lightRange.x) * -1.0 + 1.0, 0.0, 1.0);\n"
		"	lightDir             = normalize(lightDir);\n"
		"	float lightDot       = max(0.0, dot(normalDir, lightDir));\n"
		"	float lightSpecDot   = max(0.0, dot(normalDir, normalize(lightDir + viewDir)));\n"
		"\n"
		"	vec3  colorDiff      = lightColor * lightDot * lightDist + ambColor;\n"
		"	vec3  colorSpec      = lightSpecColor.rgb * pow(lightSpecDot, lightSpecColor.a) * lightDist;\n"
		"\n"
		"	gl_FragColor         = vec4(diffuse * color * colorDiff + diffuseGray * colorSpec, 1.0);\n"
		"}\n"
	};

	m_VertexShader   = CreateShader(VERTEX_SHADER, GL_VERTEX_SHADER);
	m_FragmentShader = CreateShader(FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
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

	const float CUBE_VERTICE[] =
	{
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f
	};

	const ushort CUBE_INDICES[] =
	{
		 0,  2,  1,
		 1,  2,  3,
		 4,  6,  5,
		 5,  6,  7,
		 8, 10,  9,
		 9, 10, 11,
		12, 14, 13,
		13, 14, 15,
		16, 18, 17,
		17, 18, 19,
		20, 22, 21,
		21, 22, 23
	};

	glGenBuffers(1, &m_CubeVertices);
	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 24, CUBE_VERTICE, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &m_CubeIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_CubeIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ushort) * 3 * 12, CUBE_INDICES, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_VertexBuffers.push_back(m_CubeVertices);
	m_VertexBuffers.push_back(m_CubeIndices);

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
}

void GlWidget::resizeGL(int w, int h)
{
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
		glUniform3f(m_UniformColor, object->Color.x(), object->Color.y(), object->Color.z());
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

void GlWidget::AddBox(const ISceneObjectProvider* sceneObject, const QVector3D& color, const QVector2D& tiling)
{
	m_RenderObjects.push_back(RenderObject
	{
		m_CubeVertices,
		m_CubeIndices,
		12,
		color,
		tiling,
		sceneObject
	});
}

void GlWidget::AddMesh(const ISceneObjectProvider* sceneObject, const QVector3D &color, const QVector2D &tiling)
{
}

void GlWidget::Initialize()
{
	showFullScreen();
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
