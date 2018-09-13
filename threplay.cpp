#include <cstdio>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "windows.h"

std::string loadConfig(std::string var)
{
	std::ifstream config("./threplay.config", std::ifstream::in);
	if(config.good()) {
		while(config.peek() != EOF) {
			if(config.peek() == '[') {
				config.ignore(512, '\n');
			}
			std::string id;
			std::getline(config, id, '=');
			if(id == var) {
				std::getline(config, id);
				return id;
			} else {
				config.ignore(512, '\n');
			}
		}
	}
	return "";
}

bool launchProgram(std::string game) {
	if(game != "") {
		std::string dir;
		dir = game;
		STARTUPINFO startupInfo = {0};
		PROCESS_INFORMATION processInfo = {0};

		int loc = game.find_last_of('\\');
		if(loc == std::string::npos) {
			loc = game.find_last_of('/');
		}
		if(loc == std::string::npos) {
			//no directory found
			CreateProcess(game.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
		} else {
			dir.erase(loc + 1, std::string::npos);
			CreateProcess(game.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, dir.c_str(), &startupInfo, &processInfo);
		}

		return true;
	} else {
		return false;
	}
}

int main(int argc, char *argv[])
{
	/*
	if (argc == 1) {
		bool done = false;
		printf("To open a replay, move it to the same folder as this program and drag it on top.\nEnter number of game to open: ");
		while(!done) {
			std::string game, search;
			scanf("%s", &game);
			if(game == "q") {
				done = true;
				return 0;
			}
			search = "th" + game + "g";

			game = loadConfig(search);
			if(game == "") {
				printf("Unable to find game. Type another number or \'q\' to exit: ");
			} else {
				launchProgram(game);
				done = true;
			}

		}
	} else */if(argc == 2) {
		std::string filename = argv[1];
		int namepos = filename.find(".rpy");
		bool nameFound = false;

		if(namepos == std::string::npos) {
			MessageBox(NULL, TEXT("File not detected as a Touhou Replay file. Check the filename and try again"), TEXT("Error"), MB_OK|MB_ICONERROR);
			return -1;
		}
		while (!nameFound) {
			namepos -= 1;			
			if (namepos == -1) {
				nameFound = true;
			} else if (filename[namepos] == 't' && filename[namepos + 1] == 'h') {
				nameFound = true;
			}
		}

		//in theory namepos should now be on the 't' in th on the game in the replay filename
		if (namepos == -1) {
			MessageBox(NULL, TEXT("Unable to autodetect game version. Make sure the replay includes the game number \"th#_\" in the file name."), TEXT("Error"), MB_OK|MB_ICONERROR);
			return -1;
		}
		
		nameFound = false;
		std::string conf;
		while(!nameFound) {
			if(filename[namepos] == '_') {
				//end of the line
				nameFound = true;
			} else {
				conf += filename[namepos];
				namepos++;
			}
		}

		std::string r, g;
		r = loadConfig(conf + "r");
		g = loadConfig(conf + "g");
		
		if(r != "" && g != "") {
			std::ifstream  src(filename, std::ios::binary);
			std::ofstream  dst(r, std::ios::binary);

			dst << src.rdbuf();

			launchProgram(g);
		} else {
			if(r == "") {
				MessageBox(NULL, TEXT("Unable to save replay. Check the replay settings in your config file."), TEXT("Error"), MB_OK|MB_ICONERROR);
			}
			if(g == "") {
				MessageBox(NULL, TEXT("Unable to open game. Check the game location in your config file."), TEXT("Error"), MB_OK|MB_ICONERROR);
			}
		}
	} else {
		//MessageBox(NULL, TEXT("Open this program to launch a game or drag a single Touhou replay file from the same folder onto this program to watch it"), TEXT("Alert"), MB_OK|MB_ICONINFORMATION);
		MessageBox(NULL, TEXT("Drag a single Touhou replay file from the same folder onto this program to watch it"), TEXT("Alert"), MB_OK|MB_ICONINFORMATION);
	}
	return 0;
}