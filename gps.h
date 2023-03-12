#pragma once
#include <stdint.h>
#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

using namespace std;
enum qualityIndicator{uncorrected_Coordinate=1, differentially_Correct_Coordinate, RTK_Fixed_Coordinate=4, RTK_Float_Coordinate};
string tempbuffer = "";

class GPS
{
	public:
		void send_Data(string outputBuf)
		{
			cout<<endl<<"GPS_DATATOWRITE: ";
			outputBuf+='$';					//START CHARACTER

			outputBuf+=loc_Type;
			outputBuf+=timePosFix+',';

			updateTime();					//Get current time in UTC.
			outputBuf+=timeUTC+',';
			
			outputBuf+=to_string(latitudeVal)+',';
			outputBuf+=latitudeDir+',';
			outputBuf+=to_string(longitudeVal)+',';
			outputBuf+=longitudeDir+',';

			outputBuf+=to_string(quality)+',';
			outputBuf+=to_string(satelliteCnt)+',';

			stringstream s;
			s<<HDOP_Val;
			outputBuf+=s.str()+',';

			s.clear();
			s.str("");
			s<<antennaHeight;
			outputBuf+=s.str()+',';

			outputBuf+=antennaHeight_Unit+',';

			s.clear();
			s.str("");
			s<<geoidalSeparation;
			outputBuf+=s.str()+',';
			outputBuf+=geoidalSeparationHeight_Unit+',';
			
			s.clear();
			s.str("");
			s<<correctionAge;
			outputBuf+=s.str()+',';
			outputBuf+=to_string(correctionStationID);

			outputBuf+='*';					//STOP CHARACTER

			outputBuf+=to_string(checksumVal);
			
			cout<<outputBuf<<std::endl;

		}
		void read_Data(string inputBuf)
		{
			cout<<"GPS_VARIABLES_AFTERREAD"<<endl;
			cout<<"-----------------------"<<endl;
			string rawData = inputBuf.substr(inputBuf.find('$')+1,(inputBuf.find('*')-inputBuf.find('$')-1));
			vector<string> v;
			while(rawData.length())
			{
				if(rawData.find(',') != string::npos)                  //Add GPS values to string array
				{
					v.push_back(rawData.substr(0,rawData.find(',')));	
					rawData.erase(0,rawData.find(',')+1);
				}
				else                                                        //Last GPS value in the protocol
				{
					v.push_back(rawData);
					rawData.erase();
				}
			}
			cout<<(loc_Type   			= v[0].substr(0,2))<<endl;
			cout<<(timePosFix 			= v[0].substr(2,3))<<endl;
			cout<<(timeUTC    			= v[1])<<endl;
			cout<<setprecision(11);
			cout<<(latitudeVal  			= stof(v[2]))<<endl;
			cout<<(latitudeDir     			= v[3])<<endl;
			cout<<(longitudeVal			= stof(v[4]))<<endl;
			cout<<(longitudeDir    			= v[5])<<endl;
			cout<<(quality         			= stoi(v[6]))<<endl;
			cout<<(satelliteCnt    			= stoi(v[7]))<<endl;
			cout<<setprecision(3);
			cout<<(HDOP_Val     			= stof(v[8]))<<endl;
			cout<<setprecision(7);
			cout<<(antennaHeight			= stof(v[9]))<<endl;
			cout<<(antennaHeight_Unit 	 	= v[10])<<endl;
			cout<<setprecision(5);
			cout<<(geoidalSeparation   		= stof(v[11]))<<endl;
			cout<<(geoidalSeparationHeight_Unit 	= v[12])<<endl;
			cout<<setprecision(3);
			cout<<(correctionAge   			= stof(v[13]))<<endl;
			cout<<(correctionStationID 		= stoi(v[14]))<<endl;
			cout<<(checksumVal 			= 40)<<endl;                           //cant XOR those bytes due to different variable types
			
		}

		void edit_Data()
		{
			cout<<"Do you want to edit all GPS Data? (Y for edit, N for pass)"<<endl;
			
		
			if(readString(cin)=="Y")
			{
				cout<<"GP for GPS, GL for GLONASS"<<endl;
				set_locationType(readString(cin));

				cout<<"Enter Latitude Value"<<endl;
				set_latitudeVal(readFloat(cin));
			
				cout<<"Enter Latitude Direction"<<endl;
				set_latitudeDir(readString(cin));

				cout<<"Enter Longitude Value"<<endl;
				set_longitudeVal(readFloat(cin));

				cout<<"Enter Longitude Direction"<<endl;
				set_longitudeDir(readString(cin));

				cout<<"1 for uncorrected_Coordinate, 2 for differentially_Correct_Coordinate, 4 for RTK_Fixed Coordinate, 5 for RTK_Float_Coordinate"<<endl;
				set_quality(static_cast<qualityIndicator>(readInt(cin)));

				cout<<"Enter Satellite Count"<<endl;
				set_satelliteCnt(readInt(cin));

				cout<<"Enter HDOP Value"<<endl;
				set_HDOPVal(readFloat(cin));

				cout<<"Enter Antenna Height"<<endl;
				set_antennaHeight(readFloat(cin));

				cout<<"Enter Antenna Height Unit: CM, FT or PM"<<endl;
				set_antennaHeight_Unit(readString(cin));

				cout<<"Enter Geoidal Separation"<<endl;
				set_geoidalSeparation(readFloat(cin));

				cout<<"Enter Geoidal Separation Height Unit: CM, FT, M"<<endl;
				set_geoidalSeparationHeight_Unit(readString(cin));

				cout<<"Enter Correction Age"<<endl;
				set_correctionAge(readFloat(cin));

				cout<<"Enter set Correction Station ID"<<endl;
				set_correctionStationID(readInt(cin));
			}
		}
		void set_locationType(string type)
		{
			loc_Type = type;        //enum->int, THEN int->string!!!
		}
		void updateTime()
		{
			time_t curr_time;
		       	curr_time = time(NULL);
			tm *tm_gmt = gmtime(&curr_time);
			timeUTC = to_string(tm_gmt->tm_hour)+to_string(tm_gmt->tm_min)+to_string(tm_gmt->tm_sec)+".00";
		}
		void set_latitudeVal(float value)  		{latitudeVal  = value;}
		void set_latitudeDir(string direction)    	{latitudeDir  = direction;}
		void set_longitudeVal(float value)      	{longitudeVal = value;}
		void set_longitudeDir(string direction)     	{longitudeDir = direction;}
		void set_quality(qualityIndicator qvalue)
		{
			quality = static_cast<int>(qvalue);
		}
		void set_satelliteCnt(int value)            	 {satelliteCnt = value;}
		void set_HDOPVal(float value)                    {HDOP_Val     = value;}
		void set_antennaHeight(float value)              {antennaHeight= value;}
		void set_antennaHeight_Unit(string unit)
		{
			antennaHeight_Unit = unit;        //enum->int, THEN int->string!!!
		}
		void set_geoidalSeparation(float value)          {geoidalSeparation= value;}
		void set_geoidalSeparationHeight_Unit(string unit)
		{
			geoidalSeparationHeight_Unit = unit;        //enum->int, THEN int->string!!!
		}
		void set_correctionAge(float value)              {correctionAge = value;}
		void set_correctionStationID(int value)          {correctionStationID = value;}
		void calculateChecksum()
		{
			checksumVal = 40;     //I don't know how to XOR all bytes in the GPS data, because there are floats and strings!!!
		}

	private:
		string loc_Type;
		string timePosFix;
		string timeUTC;
		float  latitudeVal;
		string latitudeDir;
		float  longitudeVal;
		string longitudeDir;
		int quality;
		int satelliteCnt;
		float HDOP_Val;
		float antennaHeight;
		string antennaHeight_Unit; 
		float geoidalSeparation;
		string geoidalSeparationHeight_Unit; 
		float correctionAge;
		int correctionStationID;
		int checksumVal;

		int readInt(std::istream& stream)
		{
			int i;
			stream >> i;
			return i;
		}
		float readFloat(std::istream& stream)
		{
			float i;
			stream >> i;
			return i;
		}
		string readString(std::istream& stream)
		{
			string i;
			stream >> i;
			return i;
		}

};
