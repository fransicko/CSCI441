Author; Michael Villafuerte
Lab: Lab 10

Q1 : They are billboarding because we are translating the box slightly in eye space.

Q2 : We see the black of the quad becasue of the dpth buffer. OpenGL draws the front of the snow flack
     knowing that it is in front of everything so it won't sraw the back of the snowflack.
	 
Q3 : We are working with primitives in eye space before they are passed to clip space.

Q4 : We need to draw back from front so that our depth buffer knows what is behind the snowflacks so it doesn'that
     render the balck background.