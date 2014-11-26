#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#define GL_GLEXT_PROTOTYPES

#include "engine/frame.h"

#include <QGLFramebufferObject>

#include <QGLWidget>
#include <QWheelEvent>
#include <QTimer>



class OSDTimer : public QObject
{
	Q_OBJECT
public:
	OSDTimer() : startAngle( 0 ), arcAngle( 270 ), arcInc( -5 ) {
		backgroundBrush = QBrush( QColor( 0, 0, 0, 168 ) );
		backgroundPen = QPen( QColor( 0, 0, 0, 168 ) );
		transparentBrush = QBrush( QColor( 0, 0, 0, 0 ) );
		timerBackgroundPen = QPen( QColor( 255, 255, 255, 32 ) );
		timerBackgroundPen.setWidth( 8 );
		timerPen = QPen( QColor( 255, 255, 255, 128 ) );
		timerPen.setWidth( 8 );
		
		connect( &timer, SIGNAL(timeout()), this, SLOT(timeOut()) );
	}
	
	void start() {
		timer.start( 40 );
	}
	
	void stop() {
		if ( timer.isActive() ) {
			timer.stop();
			emit update();
		}
	}
	
	void draw( QPainter *p, int screenWidth, int screenHeight ) {
		if ( !timer.isActive() )
			return;
		p->setPen( backgroundPen );
		p->setBrush( backgroundBrush );
		p->drawRoundedRect( QRectF( screenWidth / 2.0 - 50.0, screenHeight / 2.0 - 50.0, 100.0, 100.0 ), 15, 15, Qt::RelativeSize );
	
		p->setPen( timerBackgroundPen );
		p->setBrush( transparentBrush );
		QRectF r( screenWidth / 2.0 - 30.0, screenHeight / 2.0 - 30.0, 60.0, 60.0 );
		p->drawArc( r, 0, 16 * 360 );
		p->setPen( timerPen );
		p->drawArc( r, startAngle * 16, arcAngle * 16 );
	}
	
private slots:
	void timeOut() {
		startAngle += 10;
		arcAngle += arcInc;
		if ( startAngle >= 360 )
			startAngle = 0;
		if ( arcAngle <= 10 || arcAngle >= 340 )
			arcInc *= -1;
		emit update();
	}
	
private:
	QBrush backgroundBrush;
	QBrush transparentBrush;
	QPen backgroundPen;
	QPen timerBackgroundPen;
	QPen timerPen;
	QTimer timer;
	
	int startAngle, arcAngle;
	int arcInc;
	
signals:
	void update();
};



class OSDMessage : public QObject
{
	Q_OBJECT
public:
	OSDMessage() : messageDuration( 1 ) {
		backgroundBrush = QBrush( QColor( 0, 0, 0, 168 ) );
		backgroundPen = QPen( QColor( 0, 0, 0, 168 ) );
		foregroundPen = QPen( QColor( "white" ) );
		font = QFont("Arial", 18);
		
		timer.setSingleShot( true );
		connect( &timer, SIGNAL(timeout()), this, SLOT(timeOut()) );
	}
	// duration in seconds, 0 => unlimited
	void setMessage( const QString &text, int duration ) {
		timer.stop();
		message = text;
		if ( !message.isEmpty() && duration )
			timer.start( duration * 1000 );
		emit update();
	}
	
	void draw( QPainter *p, int screenWidth, int screenHeight ) {
		if ( message.isEmpty() )
			return;
		p->setFont( font );
		p->setPen( backgroundPen );
		p->setBrush( backgroundBrush );
		QRectF dest = QRectF( 0, 0, screenWidth, screenHeight );
		QRectF r = p->boundingRect( dest, Qt::AlignLeft, message );
		r.moveTop( screenHeight - r.height() );
		p->drawRect( r );
		p->setPen( foregroundPen );
		p->drawText( r, Qt::AlignLeft, message );
	}
	
private slots:
	void timeOut() {
		message = "";
		emit update();
	}
	
private:
	QString message;
	int messageDuration;
	
	QBrush backgroundBrush;
	QPen backgroundPen;
	QPen foregroundPen;
	QFont font;
	QTimer timer;
	
signals:
	void update();
};



class VideoWidget : public QGLWidget
{
	Q_OBJECT
public:
	VideoWidget( QWidget *parent=0 );
	~VideoWidget();

public slots:
	void showFrame( Frame *frame );
	void shot();
	void clear();
	
	void showOSDMessage( const QString &text, int duration );
	void showOSDTimer();

protected :
	void initializeGL();
	void resizeGL( int, int );
	void paintEvent( QPaintEvent *event );

	void wheelEvent( QWheelEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
	void mousePressEvent( QMouseEvent * event );

private:
	void openglDraw();
	QImage lastImage();

	double lastFrameRatio;
	Frame *lastFrame;
	
	OSDMessage osdMessage;
	OSDTimer osdTimer;

signals:
	void playPause();
	void wheelSeek( int );
	void move( int, int );
	void newSharedContext(QGLWidget*);
	void newThumbContext(QGLWidget*);
	void newFencesContext(QGLWidget*);
	void frameShown( Frame* );

	void toggleFullscreen();

	void valueChanged( int );
};
#endif // VIDEOWIDGET_H
