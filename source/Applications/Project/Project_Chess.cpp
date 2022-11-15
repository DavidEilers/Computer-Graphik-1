#include "Project_Chess.h"
#include "CGContext/CGMath.h"
#include "Utilities/CG1Helper/CG1Helper.h"
#include "Utilities/CGImageFile.h"

#include <iostream> // only for std::cin/cout
#include <string>
#include <cctype>

Project_Chess::Project_Chess(int w, int h)
	:Application (w,h),frame_res(w,h)
{
	// ...
	context.clearColor = CGVec4(0.0f, 0.0f, 0.0f, 1.0f);
	context.capabilities.depthTest= true;
	//context.capabilities.backFaceCulling = true;
	context.polygonMode = CG_FILL;
	context.capabilities.bresenham=true;
	context.capabilities.depthTest= true;
	context.capabilities.backFaceCulling=true;
	usedProg=&prog;
	setupProg(w,h);
	usedProg=&prog2;
	setupProg(w,h);

	int img_w,img_h;
	unsigned char *imgData4ub = cgImageLoad("textures/Schachbrett.tga", &img_w, &img_h);
	test_texture.allocate(img_w,img_h,imgData4ub);
	test_texture.filterMode=CG_LINEAR;
	free(imgData4ub);


	imgData4ub = cgImageLoad("textures/leuchten_gruen.tga", &img_w, &img_h);
	leuchten_texture_gruen.allocate(img_w,img_h,imgData4ub);
	leuchten_texture_gruen.filterMode=CG_LINEAR;
	free(imgData4ub);

	imgData4ub = cgImageLoad("textures/leuchten_rot.tga", &img_w, &img_h);
	leuchten_texture_rot.allocate(img_w,img_h,imgData4ub);
	leuchten_texture_rot.filterMode=CG_LINEAR;
	free(imgData4ub);

	imgData4ub = cgImageLoad("textures/leuchten_blau.tga", &img_w, &img_h);
	leuchten_texture_blau.allocate(img_w,img_h,imgData4ub);
	leuchten_texture_blau.filterMode=CG_LINEAR;
	free(imgData4ub);

	leuchten_texture = &leuchten_texture_gruen;
	usedProg = &prog;
	context.useProgram((*usedProg));

	imgData4ub = cgImageLoad("textures/x_rot.tga", &img_w, &img_h);
	x_rot.allocate(img_w,img_h,imgData4ub);
	x_rot.filterMode=CG_LINEAR;
	free(imgData4ub);
	// TODO: uncomment this for testing purposes
	//chess.resetToTestCase();
	context.clear(CG_COLOR_BUFFER_BIT | CG_DEPTH_BUFFER_BIT);
}

void Project_Chess::setupProg(int w,int h){
	(*usedProg).uniform.projectionMatrix = CGMatrix4x4::getPerspective(60.0f, ((float)w)/((float)h), 1.0f,100.0f);
	//Punktlicht
	(*usedProg).uniform.light[0].ambient = CGVec4(0.3);
	(*usedProg).uniform.light[0].diffuse = CGVec4(0.4);
	(*usedProg).uniform.light[0].specular = CGVec4(0.4);
	//Direktionales Licht
	(*usedProg).uniform.light[1].ambient = CGVec4(0.3);
	(*usedProg).uniform.light[1].diffuse = CGVec4(0.3);
	(*usedProg).uniform.light[1].specular = CGVec4(0.3);
	(*usedProg).uniform.light[1].position_es = CGVec4(-1.0f,1.0f,-1.0f,0.0f);
	//Spotlight erscheint über ausgewählter Figur
	(*usedProg).uniform.light[2].ambient = CGVec4(0.0f);
	(*usedProg).uniform.light[2].diffuse = CGVec4(0.6f,0.6f,0.0f,0.0f);
	(*usedProg).uniform.light[2].specular = CGVec4(0.4f,0.6f,0.0f,0.0f);
	(*usedProg).uniform.light[2].spot_direction = CGVec4(0.0f,-1.0f,0.0f,0.0f);
	(*usedProg).uniform.light[2].spot_exponent = 2.0f;
	(*usedProg).uniform.light[2].spot_cutoff = 10.0f;


	//Default 2 => Punktlicht und direktionales Licht
	(*usedProg).uniform.lightCount=2;
	//(*usedProg).uniform.projectionMatrix.print();
}
// helper function, only for text console demo application:
// print a human-readable repesentation of a CGChessColor value
const char *Project_Chess::color_string(const CGChessColor c)
{
	return (c == CG_CHESS_WHITE)?"white":"black";
}

// helper function, only for text console demo application:
// print a human-readable repesentation of a CGChessMove
const std::string Project_Chess::move_string(const CGChessMove& move)
{

	std::string output="XX-XX";
	output[0] = 'A' + move.from.x;
	output[1] = '1' + move.from.y;
	output[3] = 'A' + move.to.x;
	output[4] = '1' + move.to.y;
	return output;
}

// helper function, only for text console demo application:
// get a chess field from keyboard
// returns true if user gave a valid move, false if abort was requested
bool Project_Chess::get_field_from_keyboard(const std::string& prompt, CGVec2i& field)
{
	bool valid=false;
	int row,column;

	// invalid field
	field.x=-1;
	field.y=-1;

	do {
		std::cout << prompt << " ('q' to abort): " << std::flush;
		std::string user_input;
		std::cin >> user_input;

		// abort with Q or X
		if (user_input.length() >= 1) {
			if (std::tolower(user_input[0]) == 'q' || std::tolower(user_input[0]) == 'x') {
				return false;
			}
		}

		if (user_input.length() == 2) {
			column = std::tolower(user_input[0]) - 'a';
			row = user_input[1] - '1';
			if (column >= 0 && column <= 7 && row >= 0 && row <= 7) {
				valid=true;
			}
		}
	} while(!valid);

	field.x=column;
	field.y=row;
	return true;
}

// helper function, only for text console demo application:
// get a chess move from keyboard
// returns true if user gave a valid move, false if abort was requested
bool Project_Chess::get_move_from_keyboard(const std::string& prompt, CGChessMove& move)
{
	return get_field_from_keyboard(prompt + " from", move.from) && get_field_from_keyboard(prompt + " to", move.to);
}

// the actual program step which is executed in a loop
void Project_Chess::renderChessPiece(const CGChessPiece cp){

	context.vertexAttributePointer.position = chess.getPositions(cp.piece);
	context.vertexAttributePointer.normal = chess.getNormals(cp.piece);
	//context.vertexAttributePointer.texcoord = chess.getTexCoords(cp);
	const int vertCount=  chess.getNumVerts(cp.piece);

		float * color = (float *) malloc(sizeof(float)*vertCount*4);
	if(cp.color==CG_CHESS_WHITE){
		for(int i=0; i<vertCount;i++){
			color[i*4+0] = 0.1583f;
			color[i*4+1] = 0.4039f;
			color[i*4+2] = 0.0706f;
			color[i*4+3] = 1.0f;
		}
	}else{
			for(int i=0; i<vertCount;i++){/*
				color[i*4+0] = 36.0f/255.0f;
				color[i*4+1] = 27.0f/255.0f;
				color[i*4+2] = 19.0f/255.0f;
				color[i*4+3] = 1.0f;*/
					color[i*4+0] = 100.0f/255.0f;
					color[i*4+1] = 10.0f/255.0f;
					color[i*4+2] = 10.0f/255.0f;
					color[i*4+3] = 1.0f;
			}

	}
	context.vertexAttributePointer.color = color;

	context.drawArrays(chess.getPrimitve(cp.piece), 0, vertCount );
//	context.drawArrays(CG_POINTS, 0, vertCount );
	free(color);
	context.vertexAttributePointer.position = NULL;
	context.vertexAttributePointer.normal = NULL;
	context.vertexAttributePointer.color = NULL;
	context.vertexAttributePointer.texcoord = NULL;
}


void Project_Chess::renderChessBoard(){
	context.vertexAttributePointer.position = ChessBoard_pos;
	context.vertexAttributePointer.normal = ChessBoard_nom;
	context.vertexAttributePointer.texcoord = ChessBoard_texcoord;
	const int vertCount= 6;

	float * color = (float *) malloc(sizeof(float)*vertCount*4);
	for(int i=0; i<vertCount;i++){
		color[i*4+0] = 0.5f;
		color[i*4+1] = 0.5f;
		color[i*4+2] = 0.5f;
		color[i*4+3] = 1.0f;
	}
	context.vertexAttributePointer.color = color;
	context.drawArrays(CG_TRIANGLES, 0, vertCount );
//	context.drawArrays(CG_POINTS, 0, vertCount );
	free(color);
	context.vertexAttributePointer.position = NULL;
	context.vertexAttributePointer.normal = NULL;
	context.vertexAttributePointer.color = NULL;
	context.vertexAttributePointer.texcoord = NULL;
	(*usedProg).uniform.texture=NULL;

}

CGMatrix4x4 Project_Chess::calculateTranslationMatrix(int * pos){
	return CGMatrix4x4::getTranslationMatrix(pos[0]-3.5f,0.0f,-pos[1]+3.5f);
}

bool Project_Chess::renderMoveFields(std::vector<CGChessMove> moves,float height){
	CGTexture2D * texture = (*usedProg).uniform.texture;
	int field[2]={0,0};
	if(moves.empty()){
		return false;
	}
	for ( auto i = moves.begin(); i != moves.end(); i++ ) {
			//(*i).to.print();
			field[0]=(*i).to.x;
			field[1]=(*i).to.y;
			CGMatrix4x4 model = CGMatrix4x4::getTranslationMatrix(0.0f,height,0.0f)*calculateTranslationMatrix(field);
			CGMatrix4x4 modelview=view*model;
			context.capabilities.blending=true;
			(*usedProg).uniform.modelViewMatrix=modelview;
			(*usedProg).uniform.texture=texture;
			renderChessBoard();
			context.capabilities.blending=false;
	}
	return true;
}

bool Project_Chess::renderPossibleMoves(CGVec2i fieldPos){
//		printf("in renderPossibleMoves\n");
	std::vector<CGChessMove> possibleMoves = chess.board().getPossibleMoves(fieldPos);
	(*usedProg).uniform.texture=leuchten_texture;
	return renderMoveFields(possibleMoves, 0.01f);

}

void Project_Chess::button_polling(){

	//*****Mouse*****
	if(CG1Helper::isMouseButtonReleased(GLFW_MOUSE_BUTTON_LEFT)){
		leftClick=true;
	}else{
		leftClick=false;
	}
	if(CG1Helper::isMouseButtonReleased(GLFW_MOUSE_BUTTON_RIGHT)){
		renderMoves=false;
		(*usedProg).uniform.lightCount=2;
	}
	if(CG1Helper::isKeyReleased('p')){
		lockCursor = !lockCursor;
	}
		//FPS Mode
		if(lockCursor==true){
			CG1Helper::setMouseModeFPS(CG1Helper::On);
			CGVec2 mouseDelta = CG1Helper::getMouseDelta();
			mouseDelta[0]= (1.0f/context.m_frameBuffer.getWidth())*mouseDelta[0];
			mouseDelta[1]= -(1.0f/context.m_frameBuffer.getHeight())*mouseDelta[1];
			CGMatrix4x4 invertedView= view;
			invertedView.invert();
			viewVec= CGMath::normalize(viewVec+invertedView*(5.0f*(CGVec4(mouseDelta[0],mouseDelta[1],0,0))));
		}else{
			CG1Helper::setMouseModeFPS(CG1Helper::Off);
		}
	//MouseWheel
	CGVec2 mouseWheelDelta=CG1Helper::getWheelDelta();
	if( 0 < mouseWheelDelta.y || mouseWheelDelta.y < 0){
		cameraPosition+=viewVec*mouseWheelDelta.y*CG1Helper::getTimeDelta();
	}

	//*****F Keys*****
	if(CG1Helper::isKeyReleased(GLFW_KEY_F1)){
		printf("*****Tastenbelegung*****:\n");
		printf("F1\t-\tTasten Anzeigen\n");
		printf("F2\t-\tScreenshot machen\n");
		printf("F3\t-\tWirframe-Modus Ein-/Ausschalten\n");
		printf("F4\t-\tBackFaceCulling Ein-/Ausschalten\n");
		printf("F5\t-\tWechsel zwischen orthogonaler und perspektivischer Projektion\n");
		printf("WASD\t-\tIn die jeweilige Richtung bewegen\n");
		printf("c\t-\tWechsel Gouraud und Phong\n");
		//printf("l\t-\tLicht\n");
		printf("k\t-\tUmschalten zu KI-Gegner\n");
		printf("r\t-\tSpiel neustarten\n");
		printf("p\t-\tFPS-Modus Ein-/Ausschalten\n");
		printf("Scrollrad\t-\tRein-/Rauszoomen\n");
	}
	if(CG1Helper::isKeyReleased(GLFW_KEY_F2)){
		unsigned char * buff;
		context.readPixels(&buff);
		//puts("First Problem solved!");
		cgImageWriteTGA("screenshot.tga",frame_res.x,frame_res.y,buff,1);
		free(buff);
	}
	if (CG1Helper::isKeyReleased(GLFW_KEY_F3)){
		context.polygonMode = (context.polygonMode == CG_FILL?CG_LINE:CG_FILL);
	}
	if (CG1Helper::isKeyReleased(GLFW_KEY_F4)){
		context.capabilities.backFaceCulling = (context.capabilities.backFaceCulling)?false:true;
	}
	if (CG1Helper::isKeyReleased(GLFW_KEY_F5)){
		if(perspectiveProj==true){
			perspectiveProj=false;
			(*usedProg).uniform.projectionMatrix = CGMatrix4x4::getOrtho(-4.0f,4.0f,-3.0f,3.0f, 1.0f,100.0f);
		}else{
			perspectiveProj=true;
			(*usedProg).uniform.projectionMatrix = CGMatrix4x4::getPerspective(60.0f, ((float)frame_res.x)/((float)frame_res.y), 1.0f,100.0f);
		}

	}

	//*****WASD-Movement Keys****
	if (CG1Helper::isKeyPressed('w')){forward = true;}else{forward = false;}
	if (CG1Helper::isKeyPressed('s')){backward = true;}else{backward = false;}
	if(CG1Helper::isKeyPressed('a')){leftward=true;}else{leftward=false;}
	if(CG1Helper::isKeyPressed('d')){rightward=true;}else{rightward=false;}

	//cameraPosition = CGMatrix4x4::getRotationMatrixY(degree)*cameraPosition;
	if(forward==true){
		cameraPosition = cameraPosition+(viewVec*CG1Helper::getTimeDelta());
		forward=false;
	} else if(backward==true){
		cameraPosition = cameraPosition-(viewVec*CG1Helper::getTimeDelta());
	}
	if(leftward==true){
		cameraPosition= cameraPosition - (CGMath::cross(viewVec,CGVec4(0.0f,1.0f,0.0f,0.0f)))*CG1Helper::getTimeDelta();
	}else if(rightward==true){
			cameraPosition= cameraPosition + (CGMath::cross(viewVec,CGVec4(0.0f,1.0f,0.0f,0.0f)))*CG1Helper::getTimeDelta();
	}

	//*****Misc*****
		//Phong-Goroud Switch
		if (CG1Helper::isKeyReleased('c')){
			if(useProg==true){
				usedProg=&prog2;
				useProg=false;
			}else{
				usedProg=&prog;
				useProg=true;
			}
			context.useProgram((*usedProg));
		}

		if(CG1Helper::isKeyReleased('l')){
			if((*usedProg).uniform.lightCount==1)
				(*usedProg).uniform.lightCount=2;
			else
				(*usedProg).uniform.lightCount=1;
		}

		if(CG1Helper::isKeyReleased('r')){
		chess.restartGame();
			hasSomeoneWon=false;
		}
		if(CG1Helper::isKeyReleased('k')){
			againstAI=!againstAI;
		}
}

void Project_Chess::draw_game(){

	model = CGMatrix4x4::getScaleMatrix(8.0f,0.0f,8.0f);
	 modelview = view * model;
	(*usedProg).uniform.modelViewMatrix = modelview;
	(*usedProg).uniform.texture=&test_texture;
	renderChessBoard();

	int pos []= {0,0};


	//END TEST


	// This is only a demonstration for how to use
	// the various CGChess classes and methods!

	// visualize the current board
	for(int i=0;i<8;i++){
		pos[0]=i;
		for(int j=0;j<8;j++){
			pos[1]=j;
			const CGChessPiece chessPiece = chess.board().at(pos[0],pos[1]);
			//if(chessPiece!=nullptr){
			model = calculateTranslationMatrix(pos);
			modelview = view * model;
			(*usedProg).uniform.modelViewMatrix = modelview;
			renderChessPiece(chessPiece);
	//	}

		}
	}
}

CGVec2i Project_Chess::getFieldFromMouse(){
	CGVec2 mousePos = CG1Helper::getMousePos();
	CGVec4 mousePointerNear = CGVec4(mousePos[0],frame_res[1]-mousePos[1],0.0f,1.0f);
	CGVec4 mousePointerFar = CGVec4(mousePos[0],frame_res[1]-mousePos[1],1.0f,1.0f);
	CGMatrix4x4 inverseProjection= (*usedProg).uniform.projectionMatrix;
	inverseProjection.invert();
	CGMatrix4x4 viewPortTransformation = CGMatrix4x4::getIdentityMatrix();
	viewPortTransformation.at(0,0)=frame_res[0]/2.0f;
	viewPortTransformation.at(1,1)=frame_res[1]/2.0f;
	viewPortTransformation.at(2,2)=0.5f;
	viewPortTransformation.at(3,0)=frame_res[0]/2.0f;
	viewPortTransformation.at(3,1)=frame_res[1]/2.0f;
	viewPortTransformation.at(3,2)=0.5f;
	viewPortTransformation.invert();
	mousePointerNear=inverseProjection*(viewPortTransformation*mousePointerNear);
	mousePointerFar=inverseProjection*(viewPortTransformation*mousePointerFar);
	mousePointerNear=(1.0f/mousePointerNear[3])*mousePointerNear;
	mousePointerFar=(1.0f/mousePointerFar[3])*mousePointerFar;
	CGMatrix4x4 inverseView= view;
	inverseView.invert();
	mousePointerNear=inverseView*(mousePointerNear);
	mousePointerFar=inverseView*(mousePointerFar);
	CGVec4 direction= mousePointerFar-mousePointerNear;
	float r = (-mousePointerNear.y)/(direction.y);// Feld ist auf y=0 => 0 = y1+ r*y2 => r=(-y1)/y2
	CGVec4 point = mousePointerNear+r*direction;

	int field[2]={0,0};

	if(-4<point.x&& point.x <4 && -4<point.z && point.z<4 ){
		return CGVec2i((int) (point.x+4),(int) (-point.z+4));
	}else{
		return CGVec2i(-1,-1);
	}
}

void Project_Chess::outlineField(int * pos){

	model = CGMatrix4x4::getTranslationMatrix(0.0f,0.01f,0.0f)*calculateTranslationMatrix(pos);
	modelview=view*model;
	context.capabilities.blending=true;
	(*usedProg).uniform.modelViewMatrix=modelview;
	(*usedProg).uniform.texture=leuchten_texture;
	renderChessBoard();
	context.capabilities.blending=false;

}

void Project_Chess::renderCheckMateSituation(const CGChessColor color){
	std::vector <CGChessMove> moves = chess.board().getMovesThreateningKing(color);
	(*usedProg).uniform.texture=&x_rot;
	renderMoveFields(moves, 2.2f);
}

void Project_Chess::draw_gameLogic(){
	selectedFieldPos = getFieldFromMouse();
	renderCheckMateSituation(CG_CHESS_WHITE);
	renderCheckMateSituation(CG_CHESS_BLACK);
	int field[2]={0,0};
	if(hasSomeoneWon==false){
		if(renderMoves==true){
			renderPossibleMoves(possibleMoveField);
		}
		if(selectedFieldPos.x!=-1){
			field[0] = selectedFieldPos.x;
			field[1] = selectedFieldPos.y;
			outlineField(field);
			if(leftClick==true&&renderMoves==false){//
				renderMoves=renderPossibleMoves(selectedFieldPos);
				(*usedProg).uniform.light[2].spot_direction = view*CGVec4(0.0f,-1.0f,0.0f,0.0f);
				(*usedProg).uniform.light[2].position_es = view*calculateTranslationMatrix(field)*CGVec4(0.0f,3.0f,0.0f,1.0f);
				(*usedProg).uniform.lightCount=3;
				possibleMoveField=selectedFieldPos;
			}
			else if(leftClick==true&&renderMoves==true){
				CGChessMove move = CGChessMove(possibleMoveField,selectedFieldPos);
				if(chess.makeMove(move)){
					(*usedProg).uniform.lightCount=2;
					if(againstAI==false){
						if(leuchten_texture==&leuchten_texture_gruen){
							leuchten_texture = &leuchten_texture_rot;
						}else{
							leuchten_texture = &leuchten_texture_gruen;
						}
						if(chess.board().is_checkmate()||chess.board().is_draw()){
							hasSomeoneWon=true;
						}
					}else if(hasSomeoneWon==false){//No one has Won it's AIs turn
						chess.makeMove(chess.getAIMove());
						if(chess.board().is_checkmate()||chess.board().is_draw()){
							hasSomeoneWon=true;
						}
					}
					renderMoves=false;
				}
			}
		}
	}else{
		//draws a outline over the whole Field to show the Winner
		model = CGMatrix4x4::getTranslationMatrix(0.0f,0.1f,0.0f)*CGMatrix4x4::getScaleMatrix(8.0f,0.0f,8.0f);
		modelview=view*model;
		context.capabilities.blending=true;
		(*usedProg).uniform.modelViewMatrix=modelview;
		if(chess.board().is_draw()){
			(*usedProg).uniform.texture=&leuchten_texture_blau;
			renderChessBoard();
			renderCheckMateSituation(CG_CHESS_BLACK);
			renderCheckMateSituation(CG_CHESS_WHITE);
		}else{
			if(chess.board().is_checkmate(CG_CHESS_BLACK)){
				(*usedProg).uniform.texture=&leuchten_texture_gruen;
				renderChessBoard();
				renderCheckMateSituation(CG_CHESS_BLACK);
			}else{
				(*usedProg).uniform.texture=&leuchten_texture_rot;
				renderChessBoard();
				renderCheckMateSituation(CG_CHESS_WHITE);
			}

		}
	}
}

void Project_Chess::program_step()
{
	button_polling();
	context.clear(CG_COLOR_BUFFER_BIT | CG_DEPTH_BUFFER_BIT);

/*
	degree += CG1Helper::getTimeDelta()*3.0f;
	if(degree>360.0f){
		degree -= 360.0f;
	}
*/

	//calculate View
	 view = CGMatrix4x4::getLookAt(cameraPosition,cameraPosition +viewVec,CGVec4(0.0f,1.0f,0.0f,0.0f));
 	(*usedProg).uniform.light[0].position_es=view*CGVec4(0.0f,10.0f,0.0f,1.0f);
	(*usedProg).uniform.light[1].position_es = view*CGVec4(-1.0f,1.0f,-1.0f,0.0f);
	//Da Licht manipuliert wird, muss es vor dem renderen der Objekte bekannt sein
	//Da in draw_gameLogic transparente Objekte gezeichnet werden, muss die Abfrage hier geschehen
	if(renderMoves==true){
		int field[2]={0,0};
		field[0]=possibleMoveField.x;
		field[1]=possibleMoveField.y;
		(*usedProg).uniform.light[2].spot_direction = view*CGVec4(0.0f,-1.0f,0.0f,0.0f);
		(*usedProg).uniform.light[2].position_es = view*calculateTranslationMatrix(field)*CGVec4(0.0f,3.0f,0.0f,1.0f);
	}
	draw_game();
	draw_gameLogic();





/*
	point.y+=0.1f;
	const float f []={point.x,point.y,point.z,point.w};
	const float c []={1.0f,0.0f,0.0f,1.0f};
	context.vertexAttributePointer.position = f;
	context.vertexAttributePointer.color = c;
	context.drawArrays(CG_POINTS, 0, 1);
*/



	//std::cout << "*** " << color_string(chess.toMove()) << "'s turn ***" << std::endl;
/*
	// inform about "check" condition
	std::vector<CGChessMove> finishing_moves = chess.board().getMovesThreateningKing(chess.toMove());
	if (finishing_moves.size() > 0) {
		std::cout << "CHECK!" <<std::endl;
		for (size_t i=0; i<finishing_moves.size(); i++) {
			std::cout << "    threatening opponent move: " << move_string(finishing_moves[i]) << std::endl;
		}
	}


	// check for finishing conditions
	if (chess.board().is_draw()) {
		std::cout << "Draw!" << std::endl;
		gameIsFinished = true;
	}
	if (chess.board().is_checkmate(chess.toMove())) {
		std::cout << color_string(chess.toMove()) << " is CHECK MATE!" << std::endl;
		gameIsFinished = true;
	}

	if (!gameIsFinished) {
		// get the next move
		CGChessMove move;
		if (chess.toMove() == CG_CHESS_WHITE) {
			// white player is controlled by the user
			bool validMove=false;
			do {
				if (!get_move_from_keyboard("your move", move)) {
					std::cout << "exit requested!" << std::endl;
					CG1Helper::exitAppilication();
					return;
				}
				validMove = chess.testMove(move);
				if (!validMove) {
					std::cout << "invalid move!" << std::endl;
				}
			} while (!validMove);
		} else {
			// black player is simulated by the computer
			move=chess.getAIMove();
			if (!chess.testMove(move)) {
				std::cout << "I give up" << std::endl;
				return;
			}
		}

		// apply the move
		std::cout << color_string(chess.toMove()) << "'s move: " << move_string(move)<< std::endl;
		chess.makeMove(move);
	}

	if (gameIsFinished) {
		// TODO: don't do this in your project,
		// if the game is finished, you should visualize it somehow
		// but the program should go on
		CG1Helper::exitAppilication();
	}
	*/
}
