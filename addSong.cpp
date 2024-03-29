#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

using namespace std;

std::string exec(const char* cmd);
string fixString(string str);

int main(int argc, char const *argv[]) {
  const char* command;

  string wd = exec("pwd");
  wd = wd.substr(0, wd.size()-1);

  if (argc != 2) {
    cout << "Please provide the file name. Exiting with error code 1." << endl;
    return 0;
  }
  if (exec("which youtube-dl") != "/usr/bin/youtube-dl\n") {
    cout << "Please install youtube-dl.";
    cout << "Use commands similar to below to install:\n\tsudo pacman -S youtube-dl\n\tsudo apt install youtube-dl" << endl;
    cout << "Exiting with error code 2" << endl;
    return 0;
  }
  // Probe for other webm file

  string youtubeLink = argv[1];
  string ytdl_command = "youtube-dl -f bestaudio --output \"tmp-x.webm\" " + youtubeLink;
  command = ytdl_command.c_str();
  system(command);

  string remove = "rm tmp-x.webm";
  string songName, songAlbum, songArtist;
  string cofirmation;
  //system("clear");
  do {
    cout << "Enter the Following Information:"<<endl;
    cout << "Artist: ";
    getline(cin, songArtist);
    cout << "Album: ";
    getline(cin, songAlbum);
    cout << "Name: ";
    getline(cin, songName);
    cout << "Proceed? (y/n/q): ";
    getline(cin, cofirmation);
    if (cofirmation == "q") {
        cout << "Quitting..." << endl;
        command = remove.c_str();
        system(command);
        return 0;
    }
  } while (cofirmation != "y" && cofirmation != "Y");
  string filepath = "~/Music/" + fixString(songArtist) + "/" + fixString(songAlbum) + "/";
  string mkdirs = "mkdir -p " + filepath;
  command = mkdirs.c_str();
  system(command);
  string fn = fixString(songName) + ".webm";
  string move = "mv " + wd + "/tmp-x.webm" + " " + filepath + fn;
  command = move.c_str();
  system(command);

  cout << "Song Sucessfully Added!" << endl;

  return 0;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

string fixString(string str) {
  string tempString = "";
  for (int i=0; i<str.length(); i++) {
    if (str[i] == ' ' && str[i-1] != '\\') {
      tempString += "\\ ";
    } else if (str[i] == '\'' && str[i-1] != '\\') {
      tempString += "\\'";
    } else if (str[i] == '(') {
      tempString += "\\(";
    } else if (str[i] == ')') {
      tempString += "\\)";
    } else {
      tempString += str[i];
    }
  }
  return tempString;
}
