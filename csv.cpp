#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <map>

std::vector<std::map<std::string,std::string>> parse_csv(std::string filename);
std::map< std::string, std::vector<int> > check(std::vector<std::map<std::string,std::string>> data);
void print_error(std::map<std::string, std::vector<int>> error);

int main(int argc, char *argv[]){
	if(argc<2){
		std::cout<<"Please specify the csv file\n";
		exit(EXIT_FAILURE);
	}
	std::vector<std::map<std::string,std::string>> data=parse_csv(argv[1]);
    std::map< std::string, std::vector<int> > error=check(data);
	print_error(error);    
}

std::vector<std::map<std::string,std::string>> parse_csv(std::string filename){
    //file pointer
    std::ifstream fin;
    fin.open (filename); 

    std::vector<std::map<std::string,std::string>> data;
    std::string line, element;

    //get title
    std::vector<std::string> title;
    std::getline(fin,line);
    std::stringstream s(line);
    while (std::getline(s, element, ',')) 
        title.push_back(element);

    //get every row 
    while (std::getline(fin, line)) {
        std::map<std::string,std::string> row;
        std::stringstream s(line);
        int i=0;
        while (std::getline(s, element, ',')) {
            row[title[i]]=element;
            i++;
        }
        data.push_back(row);    
    }
    return data;
}

std::map< std::string, std::vector<int> > check(std::vector<std::map<std::string,std::string>> data){
    bool has_error[data.size()]={false};
    std::map< std::string, std::vector<int> > error;
    for(int i=0;i<data.size();i++) {
        if(has_error[i]) continue;
    
		//overflow
        if(std::stod(data[i]["end_time"])>100000){
            error["overflow"].push_back(i);
            has_error[i]=true;
        }   
		//start_time > end_time
        else if(std::stod(data[i]["start_time"])>std::stod(data[i]["end_time"])){
            error["start_time > end_time"].push_back(i);
            has_error[i]=true;
        }   
        else{
            for (int j=0;j<data.size();j++) {
                //same lot number
                if(!has_error[j] && i!=j && data[i]["lot_number"].compare(data[j]["lot_number"])==0){
                    //same oper
                    if(data[i]["oper"].compare(data[j]["oper"])==0){
                        error["same"].insert(error["same"].end(),{i,j});
                        has_error[i]=has_error[j]=true;
                    }
                    //different oper but overlap
                    else if( !( std::stoi(data[i]["start_time"])>std::stoi(data[j]["end_time"])|| 
                            std::stoi(data[j]["start_time"])>std::stoi(data[i]["end_time"]) ) ) {
                        error["overlap"].insert(error["overlap"].end(),{i,j});
                        has_error[i]=has_error[j]=true;
                    }
                }
            }
        }
    }
    return error;
}

void print_error(std::map<std::string, std::vector<int>> error){
    for (auto const& pair : error) {
        std::cout<<pair.first<<" total:"<<pair.second.size()<<"\n";
        for(int i=0;i<pair.second.size();i++){
            std::cout<<pair.second[i]+2<<" ";
            if(i%2!=0) std::cout<<"\n";
        }
		std::cout<<"\n";
	}
}
