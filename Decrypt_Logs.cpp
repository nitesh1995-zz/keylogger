#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdlib.h>

using namespace std;



static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}



std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}




//MAIN

int main()
{
    const std::string &SALT1 = "LM::TB::BB";
	const std::string &SALT2 = "_:/_77";
	const std::string &SALT3 = "line=wowC++";
    string codedString;
    string output;
    string file;
    cout<<" ENTER THE FILE TO BE OPEN :- ";
    cin>>file;
    ifstream myReadFile;
    myReadFile.open(file);
    if (myReadFile.is_open())
        {
            while (!myReadFile.eof())
            {
                myReadFile >> codedString;
            }
            cout<<"\n STRING READ :-";
            cout << codedString;
            std::string::size_type i;


            codedString=base64_decode(codedString);

            i = codedString.find(SALT1);
            if (i != std::string::npos)
            codedString.erase(i, SALT1.length());

            i = codedString.find(SALT3);
            if (i != std::string::npos)
            codedString.erase(i, SALT3.length());

            i = codedString.find(SALT2);
            if (i != std::string::npos)
            codedString.erase(i, SALT2.length());

            codedString=base64_decode(codedString);

             i = codedString.find(SALT1);
            if (i != std::string::npos)
            codedString.erase(i, SALT1.length());

            codedString.erase(7,SALT3.length());

            codedString=base64_decode(codedString);

             i = codedString.find(SALT3);
            if (i != std::string::npos)
            codedString.erase(i, SALT3.length());

             i = codedString.find(SALT2);
            if (i != std::string::npos)
            codedString.erase(i, SALT2.length());

            codedString.erase(0, SALT1.length());
            cout<<"\n THE OUTPUT IS :- ";
            cout<<codedString<<"\n";
            system("pause");
        }
    else{
       cout<<"!!!!!FILE NOT OPEN!!!!!\n";
       system("pause");
       }
    myReadFile.close();
    return 0;
}
