#include "EXNUMB.h"
#include <iostream>

void extract_numb(const std::string & str, std::vector<std::string>& result) {

	int size;

	std::string tempStr;

	int flag;

	int tempLenght;

	result.clear();

	result.swap(std::vector<std::string>());

	size = str.size();

	tempLenght = 0;

	flag = 0;

	for (int i = 0; i < size; i++) {

		if (' ' == str.at(i) || 
			',' == str.at(i) ||
			'{' == str.at(i) ||
			'}' == str.at(i) ||
			'[' == str.at(i) ||
			']' == str.at(i) ||
			'(' == str.at(i) ||
			')' == str.at(i) ||
			'=' == str.at(i)) {

			
			if (0 != flag) {

				tempStr.clear();
				tempStr.swap(std::string());

				for (int j = tempLenght; j > 0; j--) {
					tempStr.push_back(str[i - j]);
				}

				result.push_back(tempStr);

				flag = 0;
			}


		}
		else if( (str.at(i) >= '0' && str.at(i) <= '9') ||
			      '+' == str.at(i) ||
				  '-' == str.at(i) ||
				  '.' == str.at(i)){

				if (0 == flag) {
					flag = 1;
					tempLenght = 0;
				}
		}

		tempLenght++;
	}

}