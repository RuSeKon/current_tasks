#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

void print_cnt(vector<string>& vec)
{
  for(auto it = vec.begin(); it < vec.end(); it++)
    cout << *it << ", ";
  cout << endl;
}

void parse(vector<string>& vec, const string& str)
{
  bool state = false;
  string tmp;
  for(auto i = str.begin(); i < str.end(); i++) {
    if(*i != ' ') {
      if(!state) state = true;
      tmp.push_back(*i);
      if(*i == str.back()) vec.push_back(tmp);
    }
    if(state && *i == ' ') {
      state = false;
      vec.push_back(tmp);
      tmp.clear();
    }
  }
}

bool compareVolume(const string& a, const string& b)
{
  int resA = 0;
  int resB = 0;
  for(const auto& s : a)
    resA += s - 96;
  for(const auto& s : b)
    resB += s - 96;
  if(resA == resB) return true;
  return resA < resB;
}

string highestScoringWord(const string &str)
{
  vector<string> vec;
  parse(vec, str);
  cout << "-----------------------------START-----------------------" << "\n\n";
  cout << "Before sort: " << "vec.size = " << vec.size() << endl;
  print_cnt(vec);
  cout << endl;
  sort(vec.begin(), vec.end(), compareVolume);
  cout << "After sort: "<< "vec.size = " << vec.size() << endl;
  print_cnt(vec);
  cout << "--------------------------RESULT-------------------------" << endl << vec.back();
  return vec.back();
}

int main()
{
    string temp ("pcp optea opteaa opteaa ha hapehhpiuiotopa hapehhpiuiotopaoc uoitp stn ik ehziptegiopkiiotipa ehziptegiopkiiotipapihea ehziptegiopkiiotipapiheas sza szaepizpea szaepizpeatf otezoa otezoaa otezoaaieeppibiizen uspphpteuc lssa lssahttzo a, acstea acsteapsc t ehsesiznptgp p etpne ztpa ztpaza ztpazappzoa ztpazappzoaep a attssizma attssizmanssr s nutsa nutsaipia nutsaipiaphpeetkeort poipppisurhhiippa poipppisurhhiippassa poipppisurhhiippassauia poipppisurhhiippassauiarhgptshsoeuo pcssuoiricr a aa aaza aazaittiba aazaittibagita aazaittibagitaergt trnrts p seopsoopa seopsoopaporicb opszstonoecha opszstonoechapcsa opszstonoechapcsa ebzrheiiriu ssihonpept inipea inipeat a aepgoe oa oaia oaia a ahpoa ahpoautiepie sszna ssznau r osteiska osteiskaiseztpha osteiskaiseztpha eppseer hgpei gsea gseapp kppt a augiseo suezzcpg tia tiaiphopzissa tiaiphopzissa tts a a pen zgss zh tntcia tntciactizpr ps hhssicruinuehrsyc sezzskycri ita itahssa itahssat a asczuiia asczuiia str a asnhrposzpzpsibisied a ashn ppeehtithda ppeehtithda teesza teeszan pprtg g hoeep gpoitoa");

cout << highestScoringWord(temp) << endl;
return 0;
}


