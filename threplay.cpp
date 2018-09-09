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
	return "NA";
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("drag a single touhou replay file onto this program");
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
					r = loadConfig("th106r");
					g = loadConfig("th106g");
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

			if(r != "NA" && g != "NA") {
				std::ifstream  src(filename, std::ios::binary);
				std::ofstream  dst(r, std::ios::binary);

				dst << src.rdbuf();

				//std::cout << game << g << r << std::endl;
				
				WinExec(g.c_str(), SW_SHOW);
			}
		}
	}
}