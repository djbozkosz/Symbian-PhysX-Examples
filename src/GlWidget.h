#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QDebug>
#include <QGLWidget>
#include <QSplashScreen>
#include <QImage>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>

#include <QVector>

#include <QElapsedTimer>

#include <QPushButton>

#ifdef Q_OS_SYMBIAN
#include <GLES2/gl2.h>
#endif

#include "ISceneProvider.h"
#include "ISceneObjectProvider.h"

#include "GlConstants.h"

class GlWidget : public QGLWidget
{
	private:

	Q_OBJECT

	static const uint SHADOW_TEXTURE_WIDTH  = 1024;
	static const uint SHADOW_TEXTURE_HEIGHT = 1024;

	class RenderObject
	{
		public:

		GLuint                      Vertices;
		GLuint                      Indices;
		uint                        Faces;
		QVector4D                   Color;
		QVector2D                   Tiling;

		QMatrix4x4                  ModelWorld;

		const ISceneObjectProvider* SceneObject;
	};

	int                      m_Width;
	int                      m_Height;

	QElapsedTimer            m_Elapsed;
	uint64_t                 m_ElapsedTime;
	uint                     m_FrameCounter;

	QVector<ISceneProvider*> m_Scenes;
	int                      m_ActiveSceneIdx;

	QVector<GLuint>          m_VertexBuffers;
	QVector<RenderObject>    m_RenderObjects;

	QVector3D                m_CameraPosition;
	QMatrix4x4               m_CameraView;
	QMatrix4x4               m_CameraProjetion;

	QMatrix4x4               m_CameraShadowViewProjetion;
	QMatrix4x4               m_CameraShadowViewProjetionBias;

	GLuint                   m_IlluminationVertexShader;
	GLuint                   m_IlluminationFragmentShader;
	GLuint                   m_IlluminationShaderProgram;

	GLint                    m_IlluminationAttributePosition;
	GLint                    m_IlluminationAttributeNormal;
	GLint                    m_IlluminationAttributeTextureCoord;

	GLint                    m_IlluminationUniformMVP;
	GLint                    m_IlluminationUniformMW;
	GLint                    m_IlluminationUniformMNIT;
	GLint                    m_IlluminationUniformMVPShadow;

	GLint                    m_IlluminationUniformDiffuseTexture;
	GLint                    m_IlluminationUniformShadowTexture;
	GLint                    m_IlluminationUniformCamera;
	GLint                    m_IlluminationUniformColor;
	GLint                    m_IlluminationUniformTiling;

	GLuint                   m_DepthVertexShader;
	GLuint                   m_DepthFragmentShader;
	GLuint                   m_DepthShaderProgram;

	GLint                    m_DepthAttributePosition;

	GLint                    m_DepthUniformMVP;

	GLuint                   m_CubeVertices;
	GLuint                   m_CubeIndices;

	GLuint                   m_SphereVertices;
	GLuint                   m_SphereIndices;

	GLuint                   m_GridTexture;

	GLuint                   m_ShadowFramebuffer;
	GLuint                   m_ShadowTexture;
	GLuint                   m_ShadowDepth;

#ifdef Q_OS_WIN
	typedef GLuint(APIENTRY  *TCreateShader)            (GLenum type);
	typedef void  (APIENTRY  *TShaderSource)            (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
	typedef void  (APIENTRY  *TCompileShader)           (GLuint shader);
	typedef void  (APIENTRY  *TGetShaderiv)             (GLuint shader, GLenum pname, GLint *params);
	typedef void  (APIENTRY  *TGetShaderInfoLog)        (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef GLuint(APIENTRY  *TCreateProgram)           (void);
	typedef void  (APIENTRY  *TAttachShader)            (GLuint program, GLuint shader);
	typedef void  (APIENTRY  *TLinkProgram)             (GLuint program);
	typedef void  (APIENTRY  *TGetProgramiv)            (GLuint program, GLenum pname, GLint *params);
	typedef void  (APIENTRY  *TGetProgramInfoLog)       (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
	typedef void  (APIENTRY  *TUseProgram)              (GLuint program);
	typedef void  (APIENTRY  *TGenBuffers)              (GLsizei n, GLuint *buffers);
	typedef void  (APIENTRY  *TBindBuffer)              (GLenum target, GLuint buffer);
	typedef void  (APIENTRY  *TBufferData)              (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
	typedef void  (APIENTRY  *TActiveTexture)           (GLenum texture);
	typedef void  (APIENTRY  *TGenerateMipmap)          (GLenum target);
	typedef void  (APIENTRY  *TGenFramebuffers)         (GLsizei n, GLuint* framebuffers);
	typedef void  (APIENTRY  *TBindFramebuffer)         (GLenum target, GLuint framebuffer);
	typedef void  (APIENTRY  *TFramebufferTexture2D)    (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void  (APIENTRY  *TFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	typedef void  (APIENTRY  *TGenRenderbuffers)        (GLsizei n, GLuint* renderbuffers);
	typedef void  (APIENTRY  *TBindRenderbuffer)        (GLenum target, GLuint renderbuffer);
	typedef void  (APIENTRY  *TRenderbufferStorage)     (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	typedef GLenum(APIENTRY  *TCheckFramebufferStatus)  (GLenum target);
	typedef void  (APIENTRY  *TEnableVertexAttribArray) (GLuint index);
	typedef void  (APIENTRY  *TDisableVertexAttribArray)(GLuint index);
	typedef void  (APIENTRY  *TVertexAttribPointer)     (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
	typedef GLint (APIENTRY  *TGetAttribLocation)       (GLuint program, const GLchar *name);
	typedef GLint (APIENTRY  *TGetUniformLocation)      (GLuint program, const GLchar *name);
	typedef void  (APIENTRY  *TUniform1i)               (GLint location, GLint x);
	typedef void  (APIENTRY  *TUniform2f)               (GLint location, GLfloat x, GLfloat y);
	typedef void  (APIENTRY  *TUniform3f)               (GLint location, GLfloat x, GLfloat y, GLfloat z);
	typedef void  (APIENTRY  *TUniform4f)               (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	typedef void  (APIENTRY  *TUniformMatrix3fv)        (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void  (APIENTRY  *TUniformMatrix4fv)        (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void  (APIENTRY  *TDeleteBuffers)           (GLsizei n, const GLuint* buffers);
	typedef void  (APIENTRY  *TDeleteProgram)           (GLuint program);
	typedef void  (APIENTRY  *TDeleteShader)            (GLuint shader);
	typedef void  (APIENTRY  *TDeleteRenderbuffers)     (GLsizei n, const GLuint* renderbuffers);
	typedef void  (APIENTRY  *TDeleteFramebuffers)      (GLsizei n, const GLuint* framebuffers);

	TCreateShader             glCreateShader;
	TShaderSource             glShaderSource;
	TCompileShader            glCompileShader;
	TGetShaderiv              glGetShaderiv;
	TGetShaderInfoLog         glGetShaderInfoLog;
	TCreateProgram            glCreateProgram;
	TAttachShader             glAttachShader;
	TLinkProgram              glLinkProgram;
	TGetProgramiv             glGetProgramiv;
	TGetProgramInfoLog        glGetProgramInfoLog;
	TUseProgram               glUseProgram;
	TGenBuffers               glGenBuffers;
	TBindBuffer               glBindBuffer;
	TBufferData               glBufferData;
	TActiveTexture            glActiveTexture;
	TGenerateMipmap           glGenerateMipmap;
	TGenFramebuffers          glGenFramebuffers;
	TBindFramebuffer          glBindFramebuffer;
	TFramebufferTexture2D     glFramebufferTexture2D;
	TFramebufferRenderbuffer  glFramebufferRenderbuffer;
	TGenRenderbuffers         glGenRenderbuffers;
	TBindRenderbuffer         glBindRenderbuffer;
	TRenderbufferStorage      glRenderbufferStorage;
	TCheckFramebufferStatus   glCheckFramebufferStatus;
	TEnableVertexAttribArray  glEnableVertexAttribArray;
	TDisableVertexAttribArray glDisableVertexAttribArray;
	TVertexAttribPointer      glVertexAttribPointer;
	TGetAttribLocation        glGetAttribLocation;
	TGetUniformLocation       glGetUniformLocation;
	TUniform1i                glUniform1i;
	TUniform2f                glUniform2f;
	TUniform3f                glUniform3f;
	TUniform4f                glUniform4f;
	TUniformMatrix3fv         glUniformMatrix3fv;
	TUniformMatrix4fv         glUniformMatrix4fv;
	TDeleteBuffers            glDeleteBuffers;
	TDeleteProgram            glDeleteProgram;
	TDeleteShader             glDeleteShader;
	TDeleteRenderbuffers      glDeleteRenderbuffers;
	TDeleteFramebuffers       glDeleteFramebuffers;
#endif

	public:

	explicit GlWidget(QWidget *parent = NULL);
	virtual ~GlWidget();

	void AddScene(ISceneProvider *scene);

	protected: // QGLWidget implementation

	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	public slots:

	void ActivatePrevScene();
	void ActivateNextScene();

	void AddBox  (const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling);
	void AddSpere(const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling);
	void AddMesh (const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D& tiling);

	signals:

	void Initialized();

	void StatsUpdated_SceneIdx(uint sceneIdx);
	void StatsUpdated_RenderMs(float renderMs);
	void StatsUpdated_RenderSec(float renderMs);

	private:

	GLuint CreateShader(const char* shaderText, GLenum type);
	GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

	void CreateVertexBuffer(GLuint *vertexBuffer, GLenum type, uint size, const void* data);
	void CreateVertices(GLuint *vertexBuffer, uint count, const void* data);
	void CreateIndices(GLuint *vertexBuffer, uint count, const void* data);

	static QMatrix4x4 ComposeTransformation(const QVector3D &position, const QQuaternion &rotation = QQuaternion(), const QVector3D &scale = QVector3D(1.0f, 1.0f, 1.0f));
	static QVector<float> Matrix4x4ToFloat(const QMatrix4x4 &mat);
	static QVector<float> Matrix3x3ToFloat(const QMatrix3x3 &mat);
};

inline QMatrix4x4 GlWidget::ComposeTransformation(const QVector3D &position, const QQuaternion &rotation, const QVector3D &scale)
{
	QMatrix4x4 m;

	m.translate(position);
	m.rotate(rotation);
	m.scale(scale);

	return m;
}

inline QVector<float> GlWidget::Matrix4x4ToFloat(const QMatrix4x4 &mat)
{
	QVector<float> out(16);

#if QT_VERSION >= 0x050000 || defined(Q_OS_SYMBIAN)
	memcpy(&out[0], mat.constData(), sizeof(float) * 16);
#else
	const double *inDouble = mat.constData();
	for(uchar i = 0; i < 16; i++)
		out[i] = static_cast<float>(inDouble[i]);
#endif

	return out;
}

inline QVector<float> GlWidget::Matrix3x3ToFloat(const QMatrix3x3 &mat)
{
	QVector<float> out(9);

#if QT_VERSION >= 0x050000 || defined(Q_OS_SYMBIAN)
	memcpy(&out[0], mat.constData(), sizeof(float) * 9);
#else
	const double *inDouble = mat.constData();
	for(uchar i = 0; i < 9; i++)
		out[i] = static_cast<float>(inDouble[i]);
#endif

	return out;
}

#endif // GLWIDGET_H
