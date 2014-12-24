//g++ -std=c++11 main.cpp
#include <iostream>
#include <fstream>
#include <algorithm>
#include <list>
#include <stdio.h>
#include <unordered_map>

using namespace std;

struct Page_struct{
    int count;
    string page;

    Page_struct()
    {
        count = 0;
        page = "";
    }
};

int main()
{
	int miss, hit, frame_size = 64;
	list<Page_struct>::iterator it;
	list<Page_struct>::iterator it_t;
	list<Page_struct> List;
    unordered_map<string, list<Page_struct>::iterator> unmap;
    unordered_map<string, list<Page_struct>::iterator>::iterator got;

	string page;
	char temp[16], trace_file[20];
	fstream file;

	printf("Trace file : ");
	scanf("%s", trace_file);
	printf("size\tmiss\thit\t\tpage fault ratio\n");
	while(frame_size < 1024)
	{
		hit = miss = 0;
		List.clear();
		file.open(trace_file, ios::in);
		while(file >> temp)
		{
			file >> temp;
			temp[5] = '\0';
			page = temp;
            got = unmap.find(page);

			if( got != unmap.end() ){//found
                it = unmap[page];
                it_t = unmap[page];
                it_t++;
                (*it).count++;
                while(it_t != List.end() && (*it).count > (*it_t).count )
                {
                    int tmp_i = (*it).count;
                    (*it).count = (*it_t).count;
                    (*it_t).count = tmp_i;
                    string tmp_s = (*it).page;
                    (*it).page = (*it_t).page;
                    (*it_t).page = tmp_s;
                    unmap[(*it).page] = it;
                    unmap[(*it_t).page] = it_t;
                    it++;
                    it_t++;
                }
				++hit;
			}else {
				if(unmap.size() >= frame_size)
				{
                    unmap.erase((List.front()).page);
                    List.pop_front();
				}
				Page_struct t;
				t.count = 1;
				t.page = page;
				List.push_front(t);
				unmap[page] = List.begin();
				++miss;
			}
		}
		file.close();
        unmap.clear();
		printf("%d\t%d\t%d\t%20.9lf\n", frame_size, miss, hit, double(miss)/(hit + miss));
		frame_size <<= 1;
	}
	return 0;
}
