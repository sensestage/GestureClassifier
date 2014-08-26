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

*/

"pydongui.py".runInTerminal;


OSCGestureClassifier.path = "/home/nescivi/git/steim/GestureClassifier/build/OSCGestureClassifier/OSCGestureClassifier";
o = OSCGestureClassifier.new( 9005 );

o.start;
o.data = [0.5,0.5,0.5];

o.dataTask.play;

o.stop;


// OSCdef( \minibeedata, { |msg| msg.postln }, "/minibee/data"  );
OSCdef( \minibeedata, { |msg| if ( msg[1] == 2 ){ o.data = msg.at( [6,7,8] ); } }, "/minibee/data" );

// messages:
o.learn;
o.recognize;
o.clear;
o.threshold_( 10.0 );

// data learned:
o.learned
[ o.repetition, o.interval ]
[ o.detected, o.index, o.phase, o.distance];