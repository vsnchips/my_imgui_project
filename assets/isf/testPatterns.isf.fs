/*{
  "CREDIT": "by mojovideotech",
  "CATEGORIES": [
  	"generator",
    "2d",
  ],
  "ISFVSN" : "2",
  "DESCRIPTION": "",
  "INPUTS": [
	{
		"NAME" : 	"center",
		"TYPE" : 	"point2D",
		"DEFAULT" :	[ 0.0, 0.0 ],
		"MAX" : 	[ 3.0, 3.0 ],
     	"MIN" : 	[ -3.0, -3.0 ]
	},
	{
		"NAME" : 	"zoom",
		"TYPE" : 	"float",
		"DEFAULT" :	1.5,
		"MIN" : 	0.5,
		"MAX" : 	3.0
	},
	{
		"NAME" : 	"offset",
		"TYPE" : 	"float",
		"DEFAULT" :	0.05,
		"MIN" : 	0.01,
		"MAX" : 	0.1
	},
	{
		"NAME" : 	"glow",
		"TYPE" : 	"float",
		"DEFAULT" :	3.0,
		"MIN" : 	0.0,
		"MAX" : 	9.0
	},
	{
		"NAME" : 	"intensity",
		"TYPE" : 	"float",
		"DEFAULT" : 0.00125,
		"MIN" : 	0.0005,
		"MAX" : 	0.0025
	},
	{
		"NAME" : 	"trace",
		"TYPE" : 	"float",
		"DEFAULT" : 40.0,
		"MIN" : 	10.0,
		"MAX" : 	100.0
	},
	{
		"NAME" : 	"runtime",
		"TYPE" : 	"float",
		"DEFAULT" : 24.0,
		"MIN" : 	6.0,
		"MAX" : 	60.0
	},
	{
		"NAME" : 	"rate",
		"TYPE" : 	"float",
		"DEFAULT" : 0.5,
		"MIN" : 	-2.0,
		"MAX" : 	2.0
	}
  ]
}
*/

#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
void main()
{
	FragColor = vec4(TexCoord,0.0,0.2);
	//FragColor = vec4(gl_FragCoord.xy/1000,0.0,0.2);
}