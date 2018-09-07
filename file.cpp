#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include<errno.h>
#include<cstring>
#include <stdlib.h>
#include<sys/wait.h>
#include<vector>

#define BUF_SIZE 1000000

using namespace std;

int input(vector< vector< string> >& vec1) {
	int processes = 0;
	vector< string > vec2;
	vector<string> s;
	char cmds[5000];
	scanf("%[^\n]s", cmds);
	
	for(int i = 0; i < strlen(cmds)-1; i++) {
		if(cmds[i] == '|' && cmds[i+1] == '|') 
			cmds[i] = ' ';
	}
	
	char *token = strtok(cmds, "|");
	while(token) {
		s.push_back(string(token));
		token = strtok(NULL, "|");
	}
	for(int i = 0; i < s.size(); i++) {
		string t;
		vector<string> vec12;
		vec1.push_back(vec12);
		bool hasbrackets = false;
		//does it has brackets
		for(int j = 0; j < s[i].size(); j++) {
			if( s[i][j] == '(' ) {
				for(int k = j+1; k < s[i].size(); k++) {
					if(s[i][k] == ')'){
						hasbrackets = true;
						break;
					}
				}
				if(hasbrackets) break;
			}
		}

		if(hasbrackets){

		for(int j = 0; j < s[i].size(); j++) {
				t.push_back(s[i][j]);
				if(s[i][j] == ')'){
					vec1[vec1.size()-1].push_back(t);
					processes++;
					t = "";
				}
			}
		}
		else {
			vec1[vec1.size() - 1].push_back(s[i]);
			processes++;
		}
	}
	return processes;
}

int main() {
	vector< vector<string> > vec;
	int nc = input(vec);
	/*
	//output check	
	for(int i = 0; i < vec.size(); i++) {
		for(int j = 0; j < vec[i].size(); j++) {
			cout <<i << j<<"->"<< vec[i][j]  << " ";
		}
		cout << endl;
	}
	*/

	vector< pair<int, int> > fd;
	int fds[2];
	pipe(fds);
	fd.push_back(make_pair(fds[0], fds[1]));
	bool synch = false;
	//iterate over each group
	for(int i = 0; i < vec.size(); i++) {
		
		//only the write end of the pipe will be written by the current
		//process whil read end will be used by the next process
		pipe(fds);
		fd.push_back(make_pair(fds[0], fds[1]));
		//multiple commands in the group
		//ip:fd[fd.size()-2].first op:fd[fd.size()-1].second
			//make the entire data available int the buffer first
			char buffer[BUF_SIZE];
				int ip = fd[fd.size()-2].first;
				int op = fd[fd.size()-2].second;
				int ipn = fd[fd.size()-1].first;
				int opn = fd[fd.size()-1].second;
				int n=0;
			if(!synch) {
				n = 1;
				buffer[0] = EOF;
				synch = true;
			
			}
			else
			n = read(ip, buffer, BUF_SIZE);
			//input is now available
			//remember to close these entries
			vector<pair<int, int> > fdg;
			int fdi[2];
			
			for(int i1 = 0; i1 < vec[i].size(); i1++) {
				pipe(fdi);
				fdg.push_back(make_pair(fdi[0], fdi[1]));
			}
			//populate all the pipes
			int tpid = fork();
			if(tpid == 0) {

				//close all the pipes
				for(int j = 0; j < fd.size(); j++) {
					close(fd[j].first);
					close(fd[j].second);
				}
				for(int m = 0; m < fdg.size();m++) {
					close(fdg[m].first);
				}
				for(int m = 0; m < fdg.size(); m++) {
					write(fdg[m].second, buffer, n);
					close(fdg[m].second);
				}
				
				exit(0);
			}
			else { wait(NULL);}

			//each process should write the op to opn
			//iterate over each command of the group
			
			for(int j = 0; j < vec[i].size(); j++) {
				int pidx =fork();
				if(pidx == 0) {
					close(fdg[j].second);
					dup2(fdg[j].first, 0);
					close(ipn);	
					dup2(opn, 1);
					
					for(int j1 = 0; j1 < fd.size(); j1++) {
						close(fd[j1].first);
						close(fd[j1].second);
					}
					for(int j2 = 0; j2 < vec[i].size(); j2++) {
						close(fdg[j2].first);
						close(fdg[j2].second);
					}
					system( vec[i][j].c_str() );
					exit(0);
				}

			}

			//closing local fd's
			for(int j1 = 0; j1 < fdg.size(); j1++) {
				close(fdg[j1].first);
				close(fdg[j1].second);
			}
	
		//else
	}//for
		
		//display output
		close(fd[fd.size()-1].second);
		dup2(fd[fd.size()-1].first, 0);
		for(int i = 0; i < fd.size(); i++) {	
			close(fd[i].first);
			close(fd[i].second);
		}	
		system("cat");
	
		wait(NULL);
		return 0;
}

