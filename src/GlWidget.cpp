#include "GlWidget.h"

GlWidget::GlWidget(QWidget* parent) :
	QGLWidget(parent),
	m_CameraPosition(9.0f, -7.5f, -1.0f),
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
	m_UniformColor(-1)
{
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
		"\n"
		"void main()\n"
		"{\n"
		"	vec3 ambColor       = vec3(0.3, 0.4, 0.5);\n"
		"	vec3 lightPos       = vec3(10.0, 10.0, 10.0);\n"
		"	vec2 lightRange     = vec2(1.0, 50.0);\n"
		"	vec3 lightColor     = vec3(1.5, 1.2, 1.0);\n"
		"	vec4 lightSpecColor = vec4(1.0, 1.0, 1.0, 8.0);\n"
		"\n"
		"	vec3 diffuse        = color; /*texture2D(diffTex, texCoord).rgb;*/\n"
		"\n"
		"	vec3 viewDir        = normalize(cam - positionWorld);\n"
		"	vec3 lightDir       = lightPos - positionWorld;\n"
		"\n"
		"	float lightDist     = clamp((length(lightDir) - lightRange.x) / (lightRange.y - lightRange.x) * -1.0 + 1.0, 0.0, 1.0);\n"
		"	lightDir            = normalize(lightDir);\n"
		"	float lightDot      = max(0.0, dot(normalDir, lightDir));\n"
		"	float lightSpecDot  = max(0.0, dot(normalDir, normalize(lightDir + viewDir)));\n"
		"\n"
		"	vec3 colorDiff      = lightColor * lightDot * lightDist + ambColor;\n"
		"	vec3 colorSpec      = lightSpecColor.rgb * pow(lightSpecDot, lightSpecColor.a) * lightDist;\n"
		"\n"
		"	gl_FragColor        = vec4(diffuse * colorDiff + colorSpec, 1.0);\n"
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
	m_CameraView.rotate(80.0f, 0.0f, 1.0f, 0.0f);
	m_CameraView.translate(m_CameraPosition);

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

		glDrawElements(GL_TRIANGLES, object->Faces * 3, GL_UNSIGNED_SHORT, NULL);

		glDisableVertexAttribArray(m_AttributePosition);
		glDisableVertexAttribArray(m_AttributeNormal);
		glDisableVertexAttribArray(m_AttributeTextureCoord);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glUseProgram(0);
}

void GlWidget::AddBox(const ISceneObjectProvider* sceneObject, const QVector3D& color)
{
	m_RenderObjects.push_back(RenderObject
	{
		m_CubeVertices,
		m_CubeIndices,
		12,
		color,
		sceneObject
	});
}

void GlWidget::AddMesh(const ISceneObjectProvider* sceneObject, const QVector3D &color)
{
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
