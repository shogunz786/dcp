
/*
 * input: {
  "calendar1": [
    ["9:00", "10:30"],
    ["12:00", "13:00"],
    ["16:00", "18:00"]
  ],
  "calendar2": [
    ["10:00", "11:30"],
    ["12:30", "14:30"],
    ["14:30", "15:00"],
    ["16:00", "17:00"]
  ],
  "dailyBounds1": ["9:00", "20:00"],
  "dailyBounds2": ["10:00", "18:30"],
  "meetingDuration": 30
}
Given two calenders and daily time bounds of acceptable meetings and return time slots with meeting durtion requested.
 */
#include <vector>

using namespace std;

struct StringMeeting {
  string start;
  string end;
};

struct Meeting {
  int start;
  int end;
};

//convert 12:00 to 720 minutes 12*60+0=720
int timeToMins(string &str){
	int sepPos = str.find(":");
	int hour = stoi(str.substr(0,sepPos));
	int min = stoi(str.substr(sepPos+1));
	return hour*60+min;
}

//convert mins 720 to time 12:00 = 720/60+720%60
string minsToTime(int mins){
	int hr = mins/60;
	int min = mins%60;
	string hrStr = to_string(hr);
	string minStr=to_string(min);
	minStr=minStr.length()==1?"0"+minStr:minStr;
	return hrStr+":"+minStr;
}

void convertToMin(vector<StringMeeting> &cal1, vector<Meeting> &res){
	for(auto m: cal1){
	  int start = timeToMins(m.start);	
	  int end = timeToMins(m.end);	
		res.push_back({start,end});
	}
	
}

void arrangeInOrderCals(vector<Meeting> &a, vector<Meeting> &b, vector<Meeting> &c){
	int ai=0, bi=0;
	while(ai<a.size() && bi<b.size()){
		if(a[ai].start<b[bi].start){
			c.push_back(a[ai]);
			ai++;
		}else{
			c.push_back(b[bi]);
			bi++;
		}
	}
	while(ai<a.size()){
		c.push_back(a[ai]);
		ai++;
	}
	while(bi<b.size()){
		c.push_back(b[bi]);
		bi++;
	}
}

void mergeCal(vector<Meeting> &v, vector<Meeting> &res){
	//add first times in list
	res.push_back(v[0]);
	for(int i=1; i<v.size(); i++){
		 //if last result times overlaps with current 
		 //replace with result start and max of
		 //result end time and current end time
     if(res[res.size()-1].end >= v[i].start){
			 res[res.size()-1] = {res[res.size()-1].start,
									 max(res[res.size()-1].end,v[i].end)};
		 }else{
			 res.push_back(v[i]);
		 }		
	}
}

void findSlots(vector<Meeting> &cal, int meetingDuration,vector<StringMeeting> &res){
	for(int i=1; i<cal.size(); i++){
	  if(cal[i].start-cal[i-1].end>=meetingDuration){
			res.push_back({minsToTime(cal[i-1].end),
										 minsToTime(cal[i].start)});
		}	
	}
}


vector<StringMeeting> calendarMatching(vector<StringMeeting> calendar1,
                                       StringMeeting dailyBounds1,
                                       vector<StringMeeting> calendar2,
                                       StringMeeting dailyBounds2,
                                       int meetingDuration) {
	vector<Meeting> a;//in mins
	vector<Meeting> b;//in mins
	vector<Meeting> c;//ordered in mins
	vector<Meeting> merged;//merged in mins
	vector<StringMeeting> res;//return slots
	
	//block daily bounds from end to start calender
	calendar1.insert(calendar1.begin(),{"00:00",dailyBounds1.start});
	calendar1.push_back({dailyBounds1.end,"23:59"});
	//convert 12:00pm to 720 mins for first set of calender
	convertToMin(calendar1, a);
	
	//block daily bounds from end to start calender
	calendar2.insert(calendar2.begin(),{"00:00",dailyBounds2.start});
	calendar2.push_back({dailyBounds2.end,"23:59"});
	//convert 12:00pm to 720 mins for second set of calender
	convertToMin(calendar2, b);
	
	//now arange them in order
	arrangeInOrderCals(a,b,c);
	
	//merge overlapping time intervals
	mergeCal(c,merged);
	
	//find unblocked slots that match or exceed meetingSuration
	findSlots(merged, meetingDuration, res);
  return res;
}

