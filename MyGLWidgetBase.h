#pragma once

#include <MathTypes.hpp>

// #include <gl/glew.h>
#include <QGLWidget>
#include <gl/GLU.h>



class MyGLWidgetBase : public QGLWidget
{
	Q_OBJECT

public:
	MyGLWidgetBase(QWidget *parent);
	~MyGLWidgetBase();

protected: // system events
	virtual void	mousePressEvent(QMouseEvent * event);
	virtual void	mouseReleaseEvent(QMouseEvent * event);
	virtual void	mouseMoveEvent(QMouseEvent * event);
	virtual void	wheelEvent(QWheelEvent * event);
	virtual void	keyPressEvent(QKeyEvent * event);
protected:// system operation
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	virtual void timerEvent(QTimerEvent* event);

public://整体功能
	virtual void Release();
	virtual void Draw();
public://交互操作
	virtual void SetOperation(std::string strOperation);
	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonUp(int x, int y);
	virtual void OnLButtonDblClk(int x, int y);
	virtual void OnRButtonDown(int x, int y);
	virtual void OnRButtonUp(int x, int y);
	virtual void OnMouseMove(int x, int y);
	virtual void OnMouseWheel(short zDelta);
	virtual void OnMButtonDown(int x, int y);
	virtual void OnMButtonUp(int x, int y);
public://OpenGL操作
	virtual void screenToWorld(int sx, int sy, double& wx, double& wy);
	virtual void OnZoomOut();
	virtual void OnZoomIn();
	virtual void OnZoomBase();
	virtual void OnSize(int cx, int cy);
private:// states of the mouse
	bool m_bLBtnDown;
	bool m_bRBtnDown;
	bool m_bMBtnDown;
	IPoint2 m_ptLBtnDown;
	IPoint2 m_ptMouseCurrent;
private://Perspective
	int _nWidth;
	int _nHeight;
	DPoint3 m_pt3Eye;					// camera point
	GLdouble m_dbZNear;
	GLdouble m_dbZFar;
	GLdouble m_dbFovy;					// angle of eye open

	DPoint3 m_dbRotat;

protected:// coordinate operation

	//Prepare before drawing
	void  onPreRenderScene();
	//Post scene after drawing
	void  onPostRenderScene();
	//Model View Transform
	void onTransform();
	//Recover Model View Transform
	void onReTransform();

	//Change point between screen and world
	void screenToWorld(const IPoint2& ps, DPoint3& pw);
	void worldToScreen(DPoint3& wp, IPoint2& sp);
	//Move the current area
	void onPanScreenDelta(int cx, int cy);

	//Initialization
	void  onCreate();
	//Release
	void  onDestroy();

	//Rotate Screen
	void onScreenRotate(int x1, int y1, int x2, int y2);
	//Roll Screen
	void onScreenRoll(int cx, int cy);
	//Zoom in or zoom out
	void onZoomDelta(double dScale);
	//Zoom into a rect	
	void onZoomFitWrold(double e, double n, double w, double s);

	double GetSWScale()
	{
		return ((0 == _nHeight) ? 1.0 : (m_pt3Eye.z*(tan(m_dbFovy*PId / 180.0 / 2)*2.0)) / (double)_nHeight);
	}
	IPoint2 GetClientRect() { return IPoint2(_nWidth, _nHeight); }
	// 	IPoint2& GetClientRect(){return m_clientRect;}




private:
	int m_nState;
private:
	std::string m_strDisDir;
private:
	//set base rect
	void onBase();
	// 绘制坐标系
	void drawCoords();
public:
	virtual void OnKey(UINT nChar);
private:

	// add point by hand
	bool _bHandPoint;
private:
	short m_sPolyStyle;
	RGBAf m_clearColor;
	//text/stroke font
	// 	GTFTFont *m_pFont;
	//poly tess
	GLUtesselator *m_pGlTessObj;






public://Set Status
	// 	void SetTextSize(short sSize);
	// 	void SetSymbolSize(short sSize);
	// 	void SetSymbolTextSize(short sSize);
	void SetLineWidth(const float wt);
	void SetPointSize(const float ps);
	void SetColor(const RGBAf& col);
	void EnableBlend(bool b);
	void SetClearColor(float r, float g, float b, float a);
	void SetPolygonStyle(short style);


public://Callback
	static void CALLBACK errorCallback(GLenum errorCode);
	static void CALLBACK combineCallback(GLdouble coords[3], GLdouble *data[4], GLfloat weight[4], GLdouble **dataOut);
private:
	double SW(double s){ return s*GetSWScale(); }
	void SW(FBox3& sb)
	{
		sb._x1 *= GetSWScale();
		sb._x2 *= GetSWScale();
		sb._y1 *= GetSWScale();
		sb._y2 *= GetSWScale();
	}
private:
	//20111208
	void drawEagleView();





public:// drawing
	//20121021
	void DrawRect2D(const IPoint2& pt1, const IPoint2& pt2);
	void DrawPoint2D(const IPoint2& p);
	//20121022
	void DrawRect3D(const DPoint3& pt1, const DPoint3& pt2);
	void DrawPoint(const DPoint3& p);

	void DrawLine(const DPoint3& p1, const DPoint3& p2);
	void DrawLine3D(const DPoint3& p1, const DPoint3& p2);
	void DrawLine(const std::vector<DPoint3> &dline, bool bClose);		//draw a line
	void DrawLine3D(const std::vector<DPoint3> &dline, bool bClose);		//draw a line
	void DrawLine2D(const IPoint2& p1, const IPoint2& p2);
	//draw a polygon
	void DrawPolygon(const std::vector<std::vector<DPoint3>> &poly);
	//draw a list of polygon
	void DrawPolygons(const std::vector<std::vector<std::vector<DPoint3>>> &polys);
	void DrawRectangle(const DPoint3& p1, const DPoint3& p2);
	void DrawRectangle(const IPoint2& p1, const IPoint2& p2);
	void DrawRectangleO(const IPoint2& p1, const IPoint2& p2);
	// 	void DrawRectangleOTextAsW(const IPoint2& p1, const IPoint2& p2,std::string& str);

	void DrawCircle(DPoint3 center, double radius);
	void DrawImage(DPoint3& ptWS, DPoint3& ptEN, GLsizei imgWidth, GLsizei imgHeight, unsigned char* pData);
protected:
	virtual void draw() = 0;
	virtual void onMouse(DPoint3& pt) = 0;
};

