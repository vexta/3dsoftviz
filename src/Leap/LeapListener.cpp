#include "Leap/LeapListener.h"
#include "Leap/DirectionDetector.h"
#include "Leap/FingerPositionDetector.h"

void Leap::LeapListener::onInit( const Controller& controller )
{

}

void Leap::LeapListener::onConnect( const Controller& controller )
{
	// we put our gestures here to initialize them
	controller.enableGesture( Gesture::TYPE_CIRCLE );
	controller.enableGesture( Gesture::TYPE_KEY_TAP );
	//controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture( Gesture::TYPE_SWIPE );

	controller.config().setFloat( "Gesture.Swipe.MinLength",60.0f );
	controller.config().setFloat( "Gesture.Circle.MinArc", 6.28 );
	controller.config().save();
}

void Leap::LeapListener::onDisconnect( const Controller& controller )
{

}

void Leap::LeapListener::onExit( const Controller& controller )
{

}

void Leap::LeapListener::onFrame( const Controller& controller )
{
	Frame frame = controller.frame();
	HandList hands = frame.hands();
	Leap::DirectionDetector::Direction direction;
	bool handExtended;


	for ( int i=0; i<frame.hands().count(); i++ ) {
		if ( hands[i].isRight() ) {
			direction = Leap::DirectionDetector::getPalmDirection( hands[i] );
			leapActions.changeViewAngle( direction );
		}
		else {
			handExtended = Leap::FingerPositionDetector::isHandExtended( hands[i] );
			if ( handExtended ) {
				leapActions.startMovingForward();
			}
			else {
				leapActions.stopMovingForward();
			}
		}
	}

	//std::cout << "id: " << frame.id();
	/*
	    const GestureList gestures = frame.gestures();
	      for (int g = 0; g < gestures.count(); ++g) {
	        Gesture gesture = gestures[g];

	        HandList hands = gesture.hands();
	        Hand firstHand = hands[0];

	        switch (gesture.type()) {
	          case Gesture::TYPE_CIRCLE:
	          {
	            leapActions.zoomGraph(gesture);
	            break;
	          }
	          case Gesture::TYPE_SWIPE:
	          {
	            if(firstHand.isRight()){
	                if(leapActions.isCameraMoving)
	                    leapActions.moveCamera(gesture);
	                else
	                  leapActions.rotateGraph(gesture);
	            }
	            break;
	          }
	          case Gesture::TYPE_KEY_TAP:
	          {
	            if(firstHand.isLeft())
	                leapActions.onKeyTap(gesture);
	            break;
	          }
	          case Gesture::TYPE_SCREEN_TAP:
	          {
	            leapActions.onScreenTap(gesture);
	            break;
	          }
	          default:
	            qDebug() << "Unknown gesture type.";
	            break;
	        }
	      }*/


}

void Leap::LeapListener::onFocusGained( const Controller& controller )
{

}

void Leap::LeapListener::onFocusLost( const Controller& controller )
{

}

void Leap::LeapListener::onDeviceChange( const Controller& controller )
{
	const DeviceList devices = controller.devices();

	for ( int i = 0; i < devices.count(); ++i ) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << ( devices[i].isStreaming() ? "true" : "false" ) << std::endl;
	}

}

void Leap::LeapListener::onServiceConnect( const Controller& controller )
{

}

void Leap::LeapListener::onServiceDisconnect( const Controller& controller )
{

}
