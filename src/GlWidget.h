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

#include <QTimer>

#include <QPushButton>

#include <GLES2/gl2.h>

#include "ISceneProvider.h"
#include "ISceneObjectProvider.h"

#include "GlConstants.h"

class GlWidget : public QGLWidget
{
	private:

	Q_OBJECT

	static const uint BUTTON_WIDTH  = 50;
	static const uint BUTTON_HEIGHT = 50;

	protected:

	class RenderObject
	{
		public:

		GLuint                      Vertices;
		GLuint                      Indices;
		uint                        Faces;
		QVector4D                   Color;
		QVector2D                   Tiling;

		const ISceneObjectProvider* SceneObject;
	};

	private:

	QSplashScreen*           m_Splash;

	QPushButton*             m_PrevSceneButton;
	QPushButton*             m_NextSceneButton;

	QVector<ISceneProvider*> m_Scenes;
	int                      m_ActiveSceneIdx;

	QVector<GLuint>          m_VertexBuffers;
	QVector<RenderObject>    m_RenderObjects;

	QVector3D                m_CameraPosition;
	QMatrix4x4               m_CameraView;
	QMatrix4x4               m_CameraProjetion;
	QMatrix4x4               m_CameraViewProjetion;

	GLuint                   m_IlluminationVertexShader;
	GLuint                   m_IlluminationFragmentShader;
	GLuint                   m_IlluminationShaderProgram;

	GLint                    m_IlluminationAttributePosition;
	GLint                    m_IlluminationAttributeNormal;
	GLint                    m_IlluminationAttributeTextureCoord;

	GLint                    m_IlluminationUniformMVP;
	GLint                    m_IlluminationUniformMW;
	GLint                    m_IlluminationUniformMNIT;

	GLint                    m_IlluminationUniformDiffuseTexture;
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

	public:

	explicit GlWidget(QSplashScreen *splash = NULL, uint splashDelayMs = 5000, QWidget *parent = NULL);
	virtual ~GlWidget();

	void AddScene(ISceneProvider *scene);

	protected: // QGLWidget implementation

	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	virtual void paintGL();

	public slots:

	void AddBox  (const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling);
	void AddSpere(const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D &tiling);
	void AddMesh (const ISceneObjectProvider* sceneObject, const QVector4D &color, const QVector2D& tiling);

	private slots:

	void Initialize();

	void ActivatePrevScene();
	void ActivateNextScene();

	signals:

	void Initialized();

	private:

	static GLuint CreateShader(const char* shaderText, GLenum type);
	static GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader);

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
