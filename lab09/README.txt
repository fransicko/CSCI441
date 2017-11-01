Michael Villafuerte

Q1 : In uniform  every vertex will receive that same value. mvpMatrix should be
	uniform so that all the vertices are translated into clip space.
	
Q2 : Attributes are unique per vertex. the vertex position should be an attribute 
	becuase the position should be unique from all the other verteces.
	
Q3 : Varying variables is how data is passed from a vertex shader to fragment shader.
	 The color needs to vary based on the location of the vertex position so that is simulates
	 the color wheel going around the object.
	 
Q4 : Our program takes in the GLSL files as inputs and doesn't compile them. That is why we are
	  able to rerun our code after modifiying the GLSL.
	  
Q5 : EAch vertex already has its own color. When we run the code the shaders are interpolating bewtween
	  the colors.
	  
Q6 : It made this clearer.

Q7: 8
Q8: It was good just a little confusing towards the end with TODO #10
Q9: 3
Q10: No