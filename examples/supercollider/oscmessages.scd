/*

/OSCGestureClassifier/detected i yes/no
/OSCGestureClassifier/index i (which)
/OSCGestureClassifier/distance f (distance)
/OSCGestureClassifier/phase f (phase)

/OSCGestureClassifier/learned i (which)

/OSCGestureClassifier/repetition i (on/off)
/OSCGestureClassifier/interval i (interval)

// send:
/data f f f
/learn
/clear (clear all)
/threshold f (detection threshold)

// could also make parameters of:
// YIN -> dip threshold (0.1 -> 0.2)
// YIN -> average threshold (0.2)
// YIN -> window size (60 -> 20)
// YIN -> max delay (200)
// YIN -> min dips (2)

// startup:
// - dimensions?

// most likely gesture -> now only returned if there is a sync -> do I want that?
*/

"pydongui.py".runInTerminal;


OSCGestureClassifier.path = "/home/nescivi/git/steim/OSCGestureClassifier/build/GestureClassification/OSCGestureClassifier"
// "/home/nescivi/git/steim/GestureClassifier/build/OSCGestureClassifier/OSCGestureClassifier";
o = OSCGestureClassifier.new( 9005 );

// NetAddr.langPort

o.start( true );
o.deltaTime = 0.025;

o.dataTask.isPlaying

o.stop;


// OSCdef( \minibeedata, { |msg| msg.postln }, "/minibee/data"  );
//
OSCdef( \minibeedata, { |msg| if ( msg[1] == 2 ){ o.data = msg.at( [6,7,8] ); } }, "/minibee/data" );

(
OSCdef( \filtereddata, { |msg|
	if ( msg[2] == 1 ){
		o.data = msg.at( [3,4,5] );
	}
}, "/filteredAccelero" );
);

o.actions.put( \repetition, { |v| v.postln; } )
o.actions.put( \interval, { |v| v.postln; } )

//OSCdef( \learning, { |msg| msg.postln; }, "/OSCGestureClassifier/learning" );

// messages:
o.learn;
o.recognize;

o.clear;
o.threshold_( 10.0 );
NetAddr.langPort
o.data
// data learned:
(
Tdef( \showData, { loop{
	[ [o.learned, o.learning, o.recording ],
		[ o.repetition, o.interval ],
		[ o.detected, o.index, o.phase, o.distance]
	].postln;
	0.1.wait;
} }).play
)

Tdef( \showData ).stop;

(
w = ();
w.window = Window.new("OSCGestureClassifier", Rect( 0,0, 300, 100 ) );

w.window.front;
)


OSCdef( \testgesture, { |msg|
	msg.postln;
}, "/OSCGestureClassifier/detected/all" )

OSCdef( \testlearn,{ |msg|
	msg.postln;
}, "/OSCGestureClassifier/learning")
