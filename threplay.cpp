#include <cstdio>
#include <string>
#include <fstream>
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

int main(int argc, char *argv[])
{
	if (argc != 2) {
		MessageBox(NULL, TEXT("Drag a single Touhou replay file from the same folder onto this program"), TEXT("Alert"), MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	else {
		std::string filename = argv[1];
		int namepos = filename.find(".rpy");
		//namepos is the position of the .
		//th6_01.rpy
		//th6_ud0001.rpy
		//th128_01.rpy
		//th128_ud0001.rpy
		//th10_01.rpy
		//th10_ud0001.rpy
		int game = -1;

		bool nameFound = false, error = false;
		while (!nameFound) {
			namepos -= 1;			
			if (namepos == 1) {
				error = true;
				nameFound = true;
			} else if (filename[namepos - 2] == 't' && filename[namepos - 1] == 'h') {
				nameFound = true;
			}
		}

		if (error) {
			return -1;
		}
		else {
			error = false;
		}

		//in theory namepos should now be on the first digit of the game
		

		if (filename[namepos] == '6') {
			//eosd
			game = 6;
		} else if (filename[namepos] == '7') {
			//pcb
			game = 7;
		}
		else if (filename[namepos] == '8') {
			//in
			game = 8;
		}
		else if (filename[namepos] == '9') {
			//pofv
			game = 9;
		}
		else if (filename[namepos] == '1') {
			//mof - hsifs
			if (filename[namepos + 1] == '0') {
				//mof
				game = 10;
			}
			else if (filename[namepos + 1] == '1') {
				//sa
				game = 11;
			}
			else if (filename[namepos + 1] == '2') {
				//ufo or gfw
				if (filename[namepos + 2] == '_') {
					game = 12;
				}
				else if (filename[namepos + 2] == '8') {
					//gfw
					game = 128;
				}
			}
			else if (filename[namepos + 1] == '3') {
				//td
				game = 13;
			}
			else if (filename[namepos + 1] == '4') {
				//ddc
				game = 14;
			}
			else if (filename[namepos + 1] == '5') {
				//lolk
				game = 15;
			}
			else if (filename[namepos + 1] == '6') {
				//hsifs
				game = 16;
			}
		}

		//printf("%d %d ", game, namepos);
		//std::cout << filename[namepos];

		if (game != -1) {
			//valid game has been found
			std::string r, g, d;
			switch (game) {
				case 6:
					r = loadConfig("th06r");
					g = loadConfig("th06g");
					break;
				case 7:
					r = loadConfig("th07r");
					g = loadConfig("th07g");
					break;
				case 8:
					r = loadConfig("th08r");
					g = loadConfig("th08g");
					break;
				case 9:
					r = loadConfig("th09r");
					g = loadConfig("th09g");
					break;
				case 10:
					r = loadConfig("th10r");
					g = loadConfig("th10g");
					break;
				case 11:
					r = loadConfig("th11r");
					g = loadConfig("th11g");
					break;
				case 12:
					r = loadConfig("th12r");
					g = loadConfig("th12g");
					break;
				case 13:
					r = loadConfig("th13r");
					g = loadConfig("th13g");
					break;
				case 14:
					r = loadConfig("th14r");
					g = loadConfig("th14g");
					break;
				case 15:
					r = loadConfig("th15r");
					g = loadConfig("th15g");
					break;
				case 16:
					r = loadConfig("th16r");
					g = loadConfig("th16g");
					break;
				case 128:
					r = loadConfig("th128r");
					g = loadConfig("th128g");
					break;
			} 

			//std::cout << r << std::endl << g << std::endl;
			if(r != "" && g != "") {
				std::ifstream  src(filename, std::ios::binary);
				std::ofstream  dst(r, std::ios::binary);

				dst << src.rdbuf();

				//std::cout << game << g << r << std::endl;

				std::string d;
				d = g;
				int l = g.find_last_of('\\');
				if(l == std::string::npos) {
					l = g.find_last_of('/');
				}
				if(l == std::string::npos) {
					//no directory found
				} else {
					d.erase(l + 1, std::string::npos);
				}
				
				//MessageBox(NULL, d.c_str(), d.c_str(), MB_OK|MB_ICONINFORMATION);
				
				
				//WinExec(g.c_str(), SW_SHOW);

				STARTUPINFO startupInfo = {0};
				PROCESS_INFORMATION processInfo = {0};
				CreateProcess(g.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, d.c_str(), &startupInfo, &processInfo);
			} else {
				if(r == "") {
					MessageBox(NULL, TEXT("Unable to save replay. Check the replay settings in your config file."), TEXT("Error"), MB_OK|MB_ICONERROR);
				}
				if(g == "") {
					MessageBox(NULL, TEXT("Unable to open game. Check the game location in your config file."), TEXT("Error"), MB_OK|MB_ICONERROR);
				}
			}
			
		} else {
			//game not found
			MessageBox(NULL, TEXT("No game detected"), TEXT("Error"), MB_OK | MB_ICONERROR);
		}
	}
}