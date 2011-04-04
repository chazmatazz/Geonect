// http://forum.libcinder.org/topic/simple-hand-tracking-with-kinect-opencv

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/params/Params.h"

#include "Kinect.h"
#include "CinderOpenCv.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HandTrackingApp : public AppBasic {
public:
    void prepareSettings( Settings* settings );
    void setup();
    void update();
    void draw();
    
    params::InterfaceGl mParams;
    
    float mThreshold, mBlobMin, mBlobMax;
    float mKinectTilt;
    
    Kinect mKinect;
    gl::Texture mColorTexture, mDepthTexture, mCvTexture; 
    Surface mDepthSurface;
    
    Vec3f mTargetPosition;
};

void HandTrackingApp::prepareSettings( Settings* settings )
{
    settings->setWindowSize( 640, 720 );
}


void HandTrackingApp::setup()
{
    
    mThreshold = 70.0f;
    mBlobMin = 20.0f;
    mBlobMax = 80.0f;
    
    mParams = params::InterfaceGl( "Hand Tracking", Vec2i( 10, 10 ) );
    mParams.addParam( "Threshold", &mThreshold, "min=0.0 max=255.0 step=1.0 keyIncr=s keyDecr=w" );
    mParams.addParam( "Blob Minimum Radius", &mBlobMin, "min=1.0 max=200.0 step=1.0 keyIncr=e keyDecr=d" );
    mParams.addParam( "Blob Maximum Radius", &mBlobMax, "min=1.0 max=200.0 step=1.0 keyIncr=r keyDecr=f" );
    mParams.addParam( "Kinect Tilt", &mKinectTilt, "min=-31 max=31 keyIncr=T keyDecr=t" );
    
    mKinect = Kinect( Kinect::Device() );
    
    mTargetPosition = Vec3f::zero();
    
}


void HandTrackingApp::update()
{
    if( mKinect.checkNewDepthFrame() ){
        
        ImageSourceRef depthImage = mKinect.getDepthImage();
        
        // make a texture to display
        mDepthTexture = depthImage;
        // make a surface for opencv
        mDepthSurface = depthImage;
        
        if(mDepthSurface){
            
            // once the surface is avalable pass it to opencv
            // had trouble here with bit depth. surface comes in full color, needed to crush it down
            cv::Mat input( toOcv( Channel8u( mDepthSurface )  ) ), blurred, thresholded, thresholded2, output;
            
            cv::blur(input, blurred, cv::Size(10,10));
            
            // make two thresholded images one to display and one
            // to pass to find contours since its process alters the image
            cv::threshold( blurred, thresholded, mThreshold, 255, CV_THRESH_BINARY);
            cv::threshold( blurred, thresholded2, mThreshold, 255, CV_THRESH_BINARY);
            
            // 2d vector to store the found contours
            vector<vector<cv::Point> > contours;
            // find em
            cv::findContours(thresholded, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
            
            // convert theshold image to color for output
            // so we can draw blobs on it
            cv::cvtColor( thresholded2, output, CV_GRAY2RGB );
            
            // loop the stored contours
            for (vector<vector<cv::Point> >::iterator it=contours.begin() ; it < contours.end(); it++ ){
                
                // center abd radius for current blob
                cv::Point2f center;
                float radius;
                // convert the cuntour point to a matrix 
                vector<cv::Point> pts = *it;
                cv::Mat pointsMatrix = cv::Mat(pts);
                // pass to min enclosing circle to make the blob 
                cv::minEnclosingCircle(pointsMatrix, center, radius);
                
                cv::Scalar color( 0, 255, 0 );
                
                if (radius > mBlobMin && radius < mBlobMax) {
                    // draw the blob if it's in range
                    cv::circle(output, center, radius, color);
                    
                    //update the target position
                    mTargetPosition.x = 640 - center.x;
                    mTargetPosition.y = center.y;
                    mTargetPosition.z = 0;
                }
                
                
            }
            
            mCvTexture = gl::Texture( fromOcv( output ) );
        }
    }
    
    if( mKinect.checkNewVideoFrame() )
        mColorTexture = mKinect.getVideoImage();
    
    if( mKinectTilt != mKinect.getTilt() )
        mKinect.setTilt( mKinectTilt );
    
}

void HandTrackingApp::draw()
{
    
    gl::clear( Color( 0.5f, 0.5f, 0.5f ) );
    
    
    gl::disableDepthWrite();
    gl::disableDepthRead();
    
    glPushMatrix();
    gl::scale(Vec3f(-1, 1, 1));
    if( mColorTexture )
        gl::draw( mColorTexture, Vec2i( -640, 0));
    glPopMatrix();
    
    glPushMatrix();
    gl::scale(Vec3f(-0.5, 0.5, 1));
    if( mDepthTexture )
        gl::draw( mDepthTexture,Vec2i( -640, 920 ));
    if ( mCvTexture )
        gl::draw( mCvTexture,Vec2i( -1280, 920 ));
    glPopMatrix();
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
    
    gl::color(Colorf(1.0f, 0.0f, 0.0f));
    gl::drawSphere(mTargetPosition, 10.0f);
    
    params::InterfaceGl::draw();
    
}


CINDER_APP_BASIC( HandTrackingApp, RendererGl )
