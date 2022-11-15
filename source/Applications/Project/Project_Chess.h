#pragma once
#include "Applications/Application.h"
#include "CGContext/CGProgram.h"
#include "ShaderPrograms/Program_perVertexLighting.h"
#include "ShaderPrograms/Program_modelViewProjection.h"
#include "ShaderPrograms/Program_phong.h"
#include "CGChess.h"

static const float ChessBoard_pos[] ={
0.5f,0.0f,0.5f,
	0.5f,0.0f,-0.5f,
	-0.5f,0.0f,0.5f,

	-0.5f,0.0f,-0.5f,
	-0.5f,0.0f,0.5f,
	0.5f,0.0f,-0.5f
};

static const float ChessBoard_nom[] ={
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,

	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f
};

static const float ChessBoard_texcoord[]={
1.0f,1.0f,
	1.0f,0.0f,
	0.0f,1.0f,

	0.0f,0.0f,
	0.0f,1.0f,
	1.0f,0.0f
};


class Project_Chess :public Application {
	CGChessSystem chess;
	CGVec2i frame_res;
	CGTexture2D test_texture;
	CGTexture2D x_rot;
	CGTexture2D * leuchten_texture;
	CGTexture2D leuchten_texture_gruen;
	CGTexture2D leuchten_texture_rot;
	CGTexture2D leuchten_texture_blau;
	CGMatrix4x4 viewVecRotation = CGMatrix4x4::getIdentityMatrix();
	CGMatrix4x4 model;
	CGMatrix4x4 modelview;
	CGVec4 viewVec= CGVec4(0.0f,-1.0f,-1.0f,0.0f);
	float degree=0;
	CGVec4 cameraPosition = CGVec4(0.0f, 10.0f, 10.0f);
	CGMatrix4x4 view= CGMatrix4x4::getLookAt(cameraPosition,cameraPosition +viewVec,CGVec4(0.0f,1.0f,0.0f,0.0f));
	bool forward = false;
	bool backward = false;
	bool useProg = true;
	bool lockCursor=false;
	bool rightward = false;
	bool leftward = false;
	bool leftClick=false;
	bool renderMoves=false;
	bool hasSomeoneWon=false;
	bool perspectiveProj=true;
	bool againstAI=false;
	CGVec2i possibleMoveField=CGVec2i(0,0);
	CGProgram * usedProg;
	CGVec2i selectedFieldPos=CGVec2i(-1,-1);

//	Program_modelViewProjection prog;
	Program_phong prog;
	Program_perVertexLighting prog2;

	float * figure;//JUST FOR TESTING
	// helper functions, only for text console demo application
	const char *color_string(const CGChessColor c);
	const std::string move_string(const CGChessMove& move);
	bool get_field_from_keyboard(const std::string& prompt, CGVec2i& field);
	bool get_move_from_keyboard(const std::string& prompt, CGChessMove& move);
	void renderChessPiece(const CGChessPiece cp);
	void renderChessBoard();
	CGMatrix4x4 calculateTranslationMatrix(int * pos);
	bool renderPossibleMoves(CGVec2i fieldPos);
	void button_polling();
	void setupProg(int w, int h);
	void draw_game();
	CGVec2i getFieldFromMouse();
	void draw_gameLogic();
	void outlineField();
	void renderCheckMateSituation(const CGChessColor color);
	void outlineField(int * pos);
	bool renderMoveFields(std::vector<CGChessMove> moves, float height);



public:
	Project_Chess(int w, int h);
	void program_step();
};
