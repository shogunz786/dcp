vector<string> split(const string str){
        vector<string> v;
        stringstream ss(str);
        string token;
        while(ss>>token){
            v.push_back(token);
        }
        return v;
    }
    
    vector<int> countMentions(int numberOfUsers, vector<vector<string>>& events) {
        vector<int> res(numberOfUsers, 0);
        vector<int> offline(numberOfUsers, 0);
        sort(events.begin(), events.end(), [](const auto &a, const auto &b)
             { 
                 return ((atoi(a[1].c_str()) < atoi(b[1].c_str())) || 
                         ((atoi(a[1].c_str()) == atoi(b[1].c_str())) && (a[0] > b[0])));
             }
            );
        for(const auto &str : events){
            int ts = atoi(str[1].c_str());
            for(int k=0; k<offline.size(); k++){
                if(offline[k] && offline[k] <= ts){
                    offline[k] = 0;
                }
            }
            if(str[0] == "MESSAGE"){
                if(str[2] == "ALL"){
                    for(auto& a: res){
                        ++a;
                    }
                }else if(str[2] == "HERE"){
                    for(int k=0; k<offline.size(); k++){
                        if(offline[k]==0) res[k]++;
                    }
                }else{
                    vector<string> v = split(str[2]);
                    for(const auto& e: v){
                        int i = atoi(e.substr(2).c_str());
                        res[i]++;
                    }
                }
            }if(str[0] == "OFFLINE"){
                int i = atoi(str[2].c_str());
                offline[i]=atoi(str[1].c_str())+60;
                
            }
        }
        return res;
    }
