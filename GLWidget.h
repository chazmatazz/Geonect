#ifndef _GLWIDGET_H
#define _GLWIDGET_H

#include <QtOpenGL>

class GLWidget : public QGLWidget {
    
    Q_OBJECT // must include this if you use Qt signals/slots
    
public:
    GLWidget(QWidget *parent = NULL);
    
protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif  /* _GLWIDGET_H */
