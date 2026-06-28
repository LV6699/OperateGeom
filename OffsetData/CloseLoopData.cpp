#include "CloseLoopData.h"
using namespace ofts;
using namespace Ofte;
void CloseLoopData::MulLoopTest(DefLoop& loop)
{
DefArc c0{{268.9525,-26.3205},{281.3721,91.0133},{287.0643,31.0866},{true}};
DefSeg c1{{281.3721,91.0133},{335.4685,43.1782}};
DefArc c2{{335.4685,43.1782},{441.7331,138.6396},{347.7553,136.3769},{false}};
DefArc c3{{441.7331,138.6396},{562.4059,79.3191},{506.8798,118.7649},{true}};
DefArc c4{{562.4059,79.3191},{489.3101,-22.163},{546.2411,13.8965},{false}};
DefArc c5{{489.3101,-22.163},{383.9123,-84.8533},{463.4344,-98.6046},{true}};
DefArc c6{{383.9123,-84.8533},{358.3414,5.2911},{369.0391,-40.3733},{false}};
DefSeg c7{{358.3414,5.2911},{268.9525,-26.3205}};
loop.AddElem(DefElem(ARCTYPE, c0));
loop.AddElem(DefElem(LINETYPE, c1));
loop.AddElem(DefElem(ARCTYPE, c2));
loop.AddElem(DefElem(ARCTYPE, c3));
loop.AddElem(DefElem(ARCTYPE, c4));
loop.AddElem(DefElem(ARCTYPE, c5));
loop.AddElem(DefElem(ARCTYPE, c6));
loop.AddElem(DefElem(LINETYPE, c7));
}
void CloseLoopData::DefineAreaTest(vector<DefLoop>& loops)
{
DefLoop loop_,loop0,loop1,loop2,loop4,loop5,loop6,loop7,loop8,
loop9,loop10,loop11,loop12,loop13,loop14,loop15,loop16,loop17,
loop18,loop19,loop20,loop21,loop22,loop23,loop24,loop25,loop26,
loop27,loop28,loop29,loop30,loop31,loop32,loop33,loop34,loop35,
loop36,loop37,loop38,loop39,loop40,loop41,loop42,loop43,loop44,
loop45,loop46,loop47,loop48,loop49,loop50,loop51,loop52,loop53,
loop54,loop55,loop56,loop57,loop58,loop59,loop60,loop61,loop62;
bool is1 = true,is2 = true,is3 = true,is4 = true,
is5 = true,is6 = false,is7 = false,is8 = false;
/// loop.AddElem(DefElem(CIRCLETYPE,circle0_9));
DefSeg l0_{{13.4295,94.7282},{15.764,0.6097}};
DefSeg l1_{{15.764,0.6097},{180.2447,3.809}};
DefSeg l2_{{180.2447,3.809},{179.1003,100.3614}};
DefSeg l3_{{179.1003,100.3614},{13.4295,94.7282}};
loop_.AddElem(DefElem(LINETYPE,l0_));
loop_.AddElem(DefElem(LINETYPE,l1_));
loop_.AddElem(DefElem(LINETYPE,l2_));
loop_.AddElem(DefElem(LINETYPE,l3_));
if(is1){
DefCircle circle0_0(Point(36.5522,75.282),16.5418);
DefCircle circle0_1(Point(36.5522,75.282),13.4571);
DefCircle circle0_2(Point(36.5522,75.282),10.4853);
DefCircle circle0_3(Point(36.5522,75.282),7.45);
DefCircle circle0_4(Point(36.5522,75.282),4.7094);
DefCircle circle0_5(Point(36.5522,75.282),1.9315);
loop0.AddElem(DefElem(CIRCLETYPE,circle0_0));
loop1.AddElem(DefElem(CIRCLETYPE,circle0_1));
loop2.AddElem(DefElem(CIRCLETYPE,circle0_2));
loop2.AddElem(DefElem(CIRCLETYPE,circle0_3));
loop4.AddElem(DefElem(CIRCLETYPE,circle0_4));
loop5.AddElem(DefElem(CIRCLETYPE,circle0_5));
}
if(is2){
DefSeg l0_6{{18.8516,56.3132},{58.667,57.4317}};
DefSeg l1_6{{58.667,57.4317},{60.5299,4.0845}};
DefSeg l2_6{{60.5299,4.0845},{20.4059,2.9986}};
DefSeg l3_6{{20.4059,2.9986},{19.697,39.8951}};
DefSeg l4_6{{19.697,39.8951},{22.8991,42.6936}};
DefSeg l5_6{{22.8991,42.6936},{24.2164,38.8165}};
DefSeg l6_6{{24.2164,38.8165},{28.0937,38.9519}};
DefSeg l7_6{{28.0937,38.9519},{28.0527,45.6748}};
DefSeg l8_6{{28.0527,45.6748},{33.5685,46.0212}};
DefSeg l9_6{{33.5685,46.0212},{33.107,40.3727}};
DefSeg l10_6{{33.107,40.3727},{28.075,42.0188}};
DefSeg l11_6{{28.075,42.0188},{22.8991,42.6936}};
DefSeg l12_6{{22.8991,42.6936},{19.321,42.8725}};
DefSeg l13_6{{19.321,42.8725},{18.8516,56.3132}};
loop6.AddElem(DefElem(LINETYPE,l0_6));
loop6.AddElem(DefElem(LINETYPE,l1_6));
loop6.AddElem(DefElem(LINETYPE,l2_6));
loop6.AddElem(DefElem(LINETYPE,l3_6));
loop6.AddElem(DefElem(LINETYPE,l4_6));
loop6.AddElem(DefElem(LINETYPE,l5_6));
loop6.AddElem(DefElem(LINETYPE,l6_6));
loop6.AddElem(DefElem(LINETYPE,l7_6));
loop6.AddElem(DefElem(LINETYPE,l8_6));
loop6.AddElem(DefElem(LINETYPE,l9_6));
loop6.AddElem(DefElem(LINETYPE,l10_6));
loop6.AddElem(DefElem(LINETYPE,l11_6));
loop6.AddElem(DefElem(LINETYPE,l12_6));
loop6.AddElem(DefElem(LINETYPE,l13_6));
DefCircle circle0_7(Point(25.3001,50.8293),3.8466);
DefCircle circle0_8(Point(45.1502,48.8224),5.5774);
DefCircle circle0_9(Point(45.1502,48.8224),3.326);
loop7.AddElem(DefElem(CIRCLETYPE,circle0_7));
loop8.AddElem(DefElem(CIRCLETYPE,circle0_8));
loop9.AddElem(DefElem(CIRCLETYPE,circle0_9));
}
if(is3){
DefSeg l0_10{{56.473,38.4607},{58.0057,5.888}};
DefSeg l1_10{{58.0057,5.888},{23.1392,4.9069}};
DefSeg l2_10{{23.1392,4.9069},{22.9212,37.1531}};
DefSeg l3_10{{22.9212,37.1531},{56.473,38.4607}};
loop10.AddElem(DefElem(LINETYPE,l0_10));
loop10.AddElem(DefElem(LINETYPE,l1_10));
loop10.AddElem(DefElem(LINETYPE,l2_10));
loop10.AddElem(DefElem(LINETYPE,l3_10));
DefSeg l0_11{{53.3168,35.36},{26.8248,34.4349}};
DefSeg l1_11{{26.8248,34.4349},{26.6945,28.0416}};
DefSeg l2_11{{26.6945,28.0416},{33.6456,26.9774}};
DefSeg l3_11{{33.6456,26.9774},{35.7712,28.4661}};
DefSeg l4_11{{35.7712,28.4661},{36.074,30.7072}};
DefSeg l5_11{{36.074,30.7072},{40.0433,30.8458}};
DefSeg l6_11{{40.0433,30.8458},{43.4063,28.0799}};
DefSeg l7_11{{43.4063,28.0799},{43.4765,26.0694}};
DefSeg l8_11{{43.4765,26.0694},{46.323,25.6247}};
DefSeg l9_11{{46.323,25.6247},{48.7843,26.8532}};
DefSeg l10_11{{48.7843,26.8532},{51.9026,26.418}};
DefSeg l11_11{{51.9026,26.418},{53.5396,28.9791}};
DefSeg l12_11{{53.5396,28.9791},{53.3168,35.36}};
loop11.AddElem(DefElem(LINETYPE,l0_11));
loop11.AddElem(DefElem(LINETYPE,l1_11));
loop11.AddElem(DefElem(LINETYPE,l2_11));
loop11.AddElem(DefElem(LINETYPE,l3_11));
loop11.AddElem(DefElem(LINETYPE,l4_11));
loop11.AddElem(DefElem(LINETYPE,l5_11));
loop11.AddElem(DefElem(LINETYPE,l6_11));
loop11.AddElem(DefElem(LINETYPE,l7_11));
loop11.AddElem(DefElem(LINETYPE,l8_11));
loop11.AddElem(DefElem(LINETYPE,l9_11));
loop11.AddElem(DefElem(LINETYPE,l10_11));
loop11.AddElem(DefElem(LINETYPE,l11_11));
loop11.AddElem(DefElem(LINETYPE,l12_11));
DefCircle circle0_12(Point(31.3849,31.3013),2.1212);
DefCircle circle0_13(Point(47.9374,32.1477),2.0873);
loop12.AddElem(DefElem(CIRCLETYPE,circle0_12));
loop13.AddElem(DefElem(CIRCLETYPE,circle0_13));
DefSeg l0_14{{26.7999,7.3599},{44.7146,8.1825}};
DefSeg l1_14{{44.7146,8.1825},{50.1602,6.954}};
DefSeg l2_14{{50.1602,6.954},{55.6337,10.5737}};
DefSeg l3_14{{55.6337,10.5737},{54.3168,25.4376}};
DefSeg l4_14{{54.3168,25.4376},{26.1926,24.0178}};
DefSeg l5_14{{26.1926,24.0178},{26.7999,7.3599}};
loop14.AddElem(DefElem(LINETYPE,l0_14));
loop14.AddElem(DefElem(LINETYPE,l1_14));
loop14.AddElem(DefElem(LINETYPE,l2_14));
loop14.AddElem(DefElem(LINETYPE,l3_14));
loop14.AddElem(DefElem(LINETYPE,l4_14));
loop14.AddElem(DefElem(LINETYPE,l5_14));
DefSeg l0_15{{28.2916,9.816},{33.821,9.3785}};
DefSeg l1_15{{33.821,9.3785},{41.8149,11.037}};
DefSeg l2_15{{41.8149,11.037},{40.7202,17.5407}};
DefSeg l3_15{{40.7202,17.5407},{39.3755,17.6514}};
DefSeg l4_15{{39.3755,17.6514},{41.0669,20.0356}};
DefSeg l5_15{{41.0669,20.0356},{37.5169,23.6851}};
DefSeg l6_15{{37.5169,23.6851},{32.4696,23.2368}};
DefSeg l7_15{{32.4696,23.2368},{30.3857,20.5527}};
DefSeg l8_15{{30.3857,20.5527},{30.3676,18.125}};
DefSeg l9_15{{30.3676,18.125},{28.2916,9.816}};
loop15.AddElem(DefElem(LINETYPE,l0_15));
loop15.AddElem(DefElem(LINETYPE,l1_15));
loop15.AddElem(DefElem(LINETYPE,l2_15));
loop15.AddElem(DefElem(LINETYPE,l3_15));
loop15.AddElem(DefElem(LINETYPE,l4_15));
loop15.AddElem(DefElem(LINETYPE,l5_15));
loop15.AddElem(DefElem(LINETYPE,l6_15));
loop15.AddElem(DefElem(LINETYPE,l7_15));
loop15.AddElem(DefElem(LINETYPE,l8_15));
loop15.AddElem(DefElem(LINETYPE,l9_15));
DefCircle circle0_16(Point(36.3047,14.1156),2.7881);
DefCircle circle0_17(Point(36.3047,14.1156),1.6035);
DefCircle circle0_18(Point(30.3676,18.125),2.9553);
DefCircle circle0_19(Point(36.1104,20.8084),2.345);
DefCircle circle0_20(Point(48.4457,17.6529),4.4318);
DefCircle circle0_21(Point(48.4457,17.6529),2.0474);
loop16.AddElem(DefElem(CIRCLETYPE,circle0_16));
loop17.AddElem(DefElem(CIRCLETYPE,circle0_17));
loop18.AddElem(DefElem(CIRCLETYPE,circle0_18));
loop19.AddElem(DefElem(CIRCLETYPE,circle0_19));
loop20.AddElem(DefElem(CIRCLETYPE,circle0_20));
loop21.AddElem(DefElem(CIRCLETYPE,circle0_21));
}
if(is4){
DefSeg l0_22{{66.1589,85.2596},{66.6649,77.25}};
DefSeg l1_22{{66.6649,77.25},{57.941,75.6346}};
DefSeg l2_22{{57.941,75.6346},{54.9105,63.4425}};
DefSeg l3_22{{54.9105,63.4425},{65.5273,67.3154}};
DefSeg l4_22{{65.5273,67.3154},{57.941,75.6346}};
DefSeg l5_22{{57.941,75.6346},{66.1589,85.2596}};
DefSeg l6_22{{66.1589,85.2596},{58.6308,86.1443}};
DefSeg l7_22{{58.6308,86.1443},{63.0177,93.8048}};
DefSeg l8_22{{63.0177,93.8048},{69.7145,90.4266}};
DefSeg l9_22{{69.7145,90.4266},{66.1589,85.2596}};
loop22.AddElem(DefElem(LINETYPE,l0_22));
loop22.AddElem(DefElem(LINETYPE,l1_22));
loop22.AddElem(DefElem(LINETYPE,l2_22));
loop22.AddElem(DefElem(LINETYPE,l3_22));
loop22.AddElem(DefElem(LINETYPE,l4_22));
loop22.AddElem(DefElem(LINETYPE,l5_22));
loop22.AddElem(DefElem(LINETYPE,l6_22));
loop22.AddElem(DefElem(LINETYPE,l7_22));
loop22.AddElem(DefElem(LINETYPE,l8_22));
loop22.AddElem(DefElem(LINETYPE,l9_22));
DefCircle circle0_23(Point(66.6649,77.25),2.9247);
DefCircle circle0_24(Point(58.667,57.4317),4.0772);
DefCircle circle0_25(Point(67.3979,48.4838),5.9166);
DefCircle circle0_26(Point(70.363,25.5178),7.5987);
loop23.AddElem(DefElem(CIRCLETYPE,circle0_23));
loop24.AddElem(DefElem(CIRCLETYPE,circle0_24));
loop25.AddElem(DefElem(CIRCLETYPE,circle0_25));
loop26.AddElem(DefElem(CIRCLETYPE,circle0_26));
DefSeg l0_27{{83.674,37.5486},{69.3828,38.6711}};
DefSeg l1_27{{69.3828,38.6711},{66.8993,25.4175}};
DefSeg l2_27{{66.8993,25.4175},{73.601,23.0326}};
DefSeg l3_27{{73.601,23.0326},{83.674,37.5486}};
loop27.AddElem(DefElem(LINETYPE,l0_27));
loop27.AddElem(DefElem(LINETYPE,l1_27));
loop27.AddElem(DefElem(LINETYPE,l2_27));
loop27.AddElem(DefElem(LINETYPE,l3_27));
DefCircle circle0_28(Point(71.9585,28.533),2.5318);
loop28.AddElem(DefElem(CIRCLETYPE,circle0_28));
DefSeg l0_29{{75.9366,13.4309},{76.2438,4.6332}};
DefSeg l1_29{{76.2438,4.6332},{60.5299,4.0845}};
DefSeg l2_29{{60.5299,4.0845},{60.2227,12.8822}};
DefSeg l3_29{{60.2227,12.8822},{75.9366,13.4309}};
loop29.AddElem(DefElem(LINETYPE,l0_29));
loop29.AddElem(DefElem(LINETYPE,l1_29));
loop29.AddElem(DefElem(LINETYPE,l2_29));
loop29.AddElem(DefElem(LINETYPE,l3_29));
}
if(is5){
DefSeg l0_30{{93.0588,39.3564},{72.254,75.0672}};
DefSeg l1_30{{72.254,75.0672},{91.6773,94.5939}};
DefSeg l2_30{{91.6773,94.5939},{121.8398,92.0002}};
DefSeg l3_30{{121.8398,92.0002},{124.9536,50.6241}};
DefSeg l4_30{{124.9536,50.6241},{93.0588,39.3564}};
loop30.AddElem(DefElem(LINETYPE,l0_30));
loop30.AddElem(DefElem(LINETYPE,l1_30));
loop30.AddElem(DefElem(LINETYPE,l2_30));
loop30.AddElem(DefElem(LINETYPE,l3_30));
loop30.AddElem(DefElem(LINETYPE,l4_30));
DefCircle circle0_31(Point(95.6221,84.4633),6.2378);
DefCircle circle0_32(Point(105.0114,83.5028),6.5624);
DefCircle circle0_33(Point(83.3399,78.0407),3.0169);
loop31.AddElem(DefElem(CIRCLETYPE,circle0_31));
loop32.AddElem(DefElem(CIRCLETYPE,circle0_32));
loop33.AddElem(DefElem(CIRCLETYPE,circle0_33));
DefSeg l0_34{{87.3521,55.2598},{98.5635,62.6425}};
DefSeg l1_34{{98.5635,62.6425},{95.1026,70.7081}};
DefSeg l2_34{{95.1026,70.7081},{102.9311,71.3961}};
DefSeg l3_34{{102.9311,71.3961},{96.4389,75.5799}};
DefSeg l4_34{{96.4389,75.5799},{95.1026,70.7081}};
DefSeg l5_34{{95.1026,70.7081},{83.8586,71.38}};
DefSeg l6_34{{83.8586,71.38},{87.3521,55.2598}};
loop34.AddElem(DefElem(LINETYPE,l0_34));
loop34.AddElem(DefElem(LINETYPE,l1_34));
loop34.AddElem(DefElem(LINETYPE,l2_34));
loop34.AddElem(DefElem(LINETYPE,l3_34));
loop34.AddElem(DefElem(LINETYPE,l4_34));
loop34.AddElem(DefElem(LINETYPE,l5_34));
loop34.AddElem(DefElem(LINETYPE,l6_34));
DefSeg l0_35{{89.0912,62.1071},{90.6707,65.5982}};
DefSeg l1_35{{90.6707,65.5982},{92.5126,66.6156}};
DefSeg l2_35{{92.5126,66.6156},{94.1632,61.9434}};
DefSeg l3_35{{94.1632,61.9434},{89.0912,62.1071}};
loop35.AddElem(DefElem(LINETYPE,l0_35));
loop35.AddElem(DefElem(LINETYPE,l1_35));
loop35.AddElem(DefElem(LINETYPE,l2_35));
loop35.AddElem(DefElem(LINETYPE,l3_35));
DefSeg l0_36{{119.3044,73.514},{110.111,74.9926}};
DefSeg l1_36{{110.111,74.9926},{105.5795,65.9766}};
DefSeg l2_36{{105.5795,65.9766},{117.6489,61.9299}};
DefSeg l3_36{{117.6489,61.9299},{119.3044,73.514}};
loop36.AddElem(DefElem(LINETYPE,l0_36));
loop36.AddElem(DefElem(LINETYPE,l1_36));
loop36.AddElem(DefElem(LINETYPE,l2_36));
loop36.AddElem(DefElem(LINETYPE,l3_36));
DefCircle circle0_37(Point(112.6913,70.0778),2.3652);
loop37.AddElem(DefElem(CIRCLETYPE,circle0_37));
DefSeg l0_38{{120.9678,68.1131},{103.233,56.3482}};
DefSeg l1_38{{103.233,56.3482},{107.3779,52.9239}};
DefSeg l2_38{{107.3779,52.9239},{123.2779,54.9968}};
DefSeg l3_38{{123.2779,54.9968},{120.9678,68.1131}};
loop38.AddElem(DefElem(LINETYPE,l0_38));
loop38.AddElem(DefElem(LINETYPE,l1_38));
loop38.AddElem(DefElem(LINETYPE,l2_38));
loop38.AddElem(DefElem(LINETYPE,l3_38));
DefCircle circle0_39(Point(102.7217,52.2567),8.348);
DefCircle circle0_40(Point(102.7217,52.2567),5.5649);
DefCircle circle0_41(Point(102.7217,52.2567),2.3053);
loop39.AddElem(DefElem(CIRCLETYPE,circle0_39));
loop40.AddElem(DefElem(CIRCLETYPE,circle0_40));
loop41.AddElem(DefElem(CIRCLETYPE,circle0_41));
}
vector<DefLoop>v{loop_,loop0,loop1,loop2,loop2,loop4,loop5,loop6,loop7,loop8,
loop9,loop10,loop11,loop12,loop13,loop14,loop15,loop16,loop17,
loop18,loop19,loop20,loop21,loop22,loop23,loop24,loop25,loop26,
loop27,loop28,loop29,loop30,loop31,loop32,loop33,loop34,loop35,
loop36,loop37,loop38,loop39,loop40,loop41};
loops = v;
}
void CloseLoopData::ClipSample(std::vector<ofts::DefLoop>& sloops,std::vector<ofts::DefLoop>& cloops)
{
ofts::DefLoop sloop0,cloop0,cloop1,cloop2;
DefSeg l0_0{{75.6816,32.2267},{8.1595,20.2752}};
DefSeg l1_0{{8.1595,20.2752},{69.3908,-12.691}};
DefSeg l2_0{{69.3908,-12.691},{38.237,49.7976}};
DefSeg l3_0{{38.237,49.7976},{25.355,-21.2722}};
DefSeg l4_0{{25.355,-21.2722},{75.6816,32.2267}};
sloop0.AddElem(DefElem(l0_0));
sloop0.AddElem(DefElem(l1_0));
sloop0.AddElem(DefElem(l2_0));
sloop0.AddElem(DefElem(l3_0));
sloop0.AddElem(DefElem(l4_0));
DefSeg l0_1{{24.0297,36.1485},{52.1846,42.2705}};
DefArc a1_1{{52.1846,42.2705},{61.7915,23.4822},{48.0118,28.2866},true};
DefArc a2_1{{61.7915,23.4822},{69.6627,6.7758},{73.1274,18.6157},false};
DefArc a3_1{{69.6627,6.7758},{63.9776,-1.8331},{68.1792,1.5739},true};
DefSeg l4_1{{63.9776,-1.8331},{46.7198,-18.2276}};
DefSeg l5_1{{46.7198,-18.2276},{33.5752,-12.5339}};
DefSeg l6_1{{33.5752,-12.5339},{28.094,-6.161}};
DefSeg l7_1{{28.094,-6.161},{19.1167,14.376}};
DefSeg l8_1{{19.1167,14.376},{22.4242,22.8001}};
DefSeg l9_1{{22.4242,22.8001},{24.0297,36.1485}};
DefSeg l10_1{{24.0297,36.1485},{24.0297,36.1485}};
DefCircle circle0_2(Point(42.8934,13.9593),23.106);
DefCircle circle0_3(Point(42.7686,14.0764),5.6037);
cloop0.AddElem(DefElem(l0_1));
cloop0.AddElem(DefElem(a1_1));
cloop0.AddElem(DefElem(a2_1));
cloop0.AddElem(DefElem(a3_1));
cloop0.AddElem(DefElem(l4_1));
cloop0.AddElem(DefElem(l5_1));
cloop0.AddElem(DefElem(l6_1));
cloop0.AddElem(DefElem(l7_1));
cloop0.AddElem(DefElem(l8_1));
cloop0.AddElem(DefElem(l9_1));
cloop0.AddElem(DefElem(l10_1));
cloop1.AddElem(DefElem(circle0_2));
cloop2.AddElem(DefElem(circle0_3));
sloops.push_back(sloop0);
cloops.push_back(cloop1);
//cloops.push_back(cloop2);
}

void CloseLoopData::OutputStruct(ofts::GeomArea& region)
{
    ofts::DefLoop loop_;
DefSeg l0_0{{66.128,7.5504},{12.7682,123.4723}};loop_.AddElem(l0_0);
DefSeg l1_0{{12.7682,123.4723},{33.2598,130.9714}};loop_.AddElem(l1_0);
DefSeg l2_0{{33.2598,130.9714},{59.7525,112.0618}};loop_.AddElem(l2_0);
DefArc arc3_0{{59.7525,112.0618},{60.574,82.8079},{51.8957,97.2027},false};loop_.AddElem(arc3_0);
DefSeg l4_0{{60.574,82.8079},{82.7569,111.084}};loop_.AddElem(l4_0);
DefArc arc5_0{{82.7569,111.084},{78.1818,126.3886},{79.066,118.3168},false};loop_.AddElem(arc5_0);
DefSeg l6_0{{78.1818,126.3886},{65.9459,120.3388}};loop_.AddElem(l6_0);
DefSeg l7_0{{65.9459,120.3388},{58.5512,133.3617}};loop_.AddElem(l7_0);
DefArc arc8_0{{58.5512,133.3617},{90.462,134.3194},{74.6588,128.7712},true};loop_.AddElem(arc8_0);
DefArc arc9_0{{90.462,134.3194},{120.2026,110.8526},{121.1538,142.6375},false};loop_.AddElem(arc9_0);
DefSeg l10_0{{120.2026,110.8526},{205.1408,114.4389}};loop_.AddElem(l10_0);
DefSeg l11_0{{205.1408,114.4389},{203.1491,58.6358}};loop_.AddElem(l11_0);
DefSeg l12_0{{203.1491,58.6358},{209.6617,26.0441}};loop_.AddElem(l12_0);
DefSeg l13_0{{209.6617,26.0441},{173.6048,16.998}};loop_.AddElem(l13_0);
DefSeg l14_0{{173.6048,16.998},{66.128,7.5504}};loop_.AddElem(l14_0);
region._bndLoop = loop_;

ofts::DefLoop loop0,loop1;
DefCircle circle_0(Point(96.4173,79.3781),7.3106);loop0.AddElem(circle_0);
DefCircle circle_1(Point(117.4602,80.0565),8.0277);loop1.AddElem(circle_1);
region.AddILoop(loop0);region.AddILoop(loop1);
ofts::DefLoop loop2;
DefSeg l0_1{{179.468,81.9233},{174.9706,89.1145}};loop2.AddElem(l0_1);
DefSeg l1_1{{174.9706,89.1145},{163.6832,87.4508}};loop2.AddElem(l1_1);
DefSeg l2_1{{163.6832,87.4508},{162.3778,79.4834}};loop2.AddElem(l2_1);
DefSeg l3_1{{162.3778,79.4834},{173.2263,68.5178}};loop2.AddElem(l3_1);
DefSeg l4_1{{173.2263,68.5178},{172.5291,65.2295}};loop2.AddElem(l4_1);
DefSeg l5_1{{172.5291,65.2295},{156.2848,67.8514}};loop2.AddElem(l5_1);
DefSeg l6_1{{156.2848,67.8514},{155.8952,92.1182}};loop2.AddElem(l6_1);
DefSeg l7_1{{155.8952,92.1182},{183.2944,92.3778}};loop2.AddElem(l7_1);
DefSeg l8_1{{183.2944,92.3778},{186.7924,67.2841}};loop2.AddElem(l8_1);
DefSeg l9_1{{186.7924,67.2841},{177.3524,68.4228}};loop2.AddElem(l9_1);
DefSeg l10_1{{177.3524,68.4228},{179.468,81.9233}};loop2.AddElem(l10_1);region.AddILoop(loop2);
ofts::DefLoop loop3,loop4;
DefCircle circle_3(Point(153.9788,30.4319),5.6871);loop3.AddElem(circle_3);
DefCircle circle_4(Point(83.2899,27.0852),5.534);loop4.AddElem(circle_4);
region.AddILoop(loop3);region.AddILoop(loop4);
ofts::DefLoop loop5;
DefSeg l0_2{{173.2736,43.9256},{178.367,43.7548}};loop5.AddElem(l0_2);
DefArc arc1_2{{178.367,43.7548},{192.1194,39.8053},{185.0462,41.0941},false};loop5.AddElem(arc1_2);
DefSeg l2_2{{192.1194,39.8053},{198.8899,39.0987}};loop5.AddElem(l2_2);
DefSeg l3_2{{198.8899,39.0987},{199.3047,32.2072}};loop5.AddElem(l3_2);
DefSeg l4_2{{199.3047,32.2072},{173.0578,34.3019}};loop5.AddElem(l4_2);
DefSeg l5_2{{173.0578,34.3019},{173.2736,43.7548}};loop5.AddElem(l5_2);region.AddILoop(loop5);
ofts::DefLoop loop6;
DefSeg l0_21{{172.1566,60.0357},{169.702,53.2775}};loop6.AddElem(l0_21);
DefSeg l1_21{{169.702,53.2775},{165.1246,53.5676}};loop6.AddElem(l1_21);
DefSeg l2_21{{165.1246,53.5676},{167.1354,60.4032}};loop6.AddElem(l2_21);
DefSeg l3_21{{167.1354,60.4032},{172.1566,60.0357}};loop6.AddElem(l3_21);region.AddILoop(loop6);
ofts::DefLoop loop7,loop8;
DefCircle circle7(Point(139.5741,25.1209),2.5454);loop7.AddElem(circle7);
DefCircle circle8(Point(78.4097,48.3354),6.2182);loop8.AddElem(circle8);
region.AddILoop(loop7);region.AddILoop(loop8);
ofts::DefLoop loop9;
DefSeg l0_3{{150.4467,51.5449},{149.8074,49.3592}};loop9.AddElem(l0_3);
DefSeg l1_3{{149.8074,49.3592},{123.606,50.4094}};loop9.AddElem(l1_3);
DefSeg l2_3{{123.606,50.4094},{124.4416,55.6587}};loop9.AddElem(l2_3);
DefSeg l3_3{{124.4416,55.6587},{143.6265,54.5936}};loop9.AddElem(l3_3);
DefSeg l4_3{{143.6265,54.5936},{127.2111,89.9814}};loop9.AddElem(l4_3);
DefSeg l5_3{{127.2111,89.9814},{131.1653,91.4271}};loop9.AddElem(l5_3);
DefSeg l6_3{{131.1653,91.4271},{150.4467,51.5449}};loop9.AddElem(l6_3);region.AddILoop(loop9);
ofts::DefLoop loop10;
DefSeg l0_4{{134.9981,40.1654},{127.2505,42.9008}};loop10.AddElem(l0_4);
DefSeg l1_4{{127.2505,42.9008},{128.2697,44.7801}};loop10.AddElem(l1_4);
DefSeg l2_4{{128.2697,44.7801},{135.927,44.5681}};loop10.AddElem(l2_4);
DefSeg l3_4{{135.927,44.5681},{134.9981,40.1654}};loop10.AddElem(l3_4);region.AddILoop(loop10);
ofts::DefLoop loop11;
DefSeg l0_41{{123.1039,34.761},{101.0993,38.8281}};loop11.AddElem(l0_41);
DefSeg l1_41{{101.0993,38.8281},{103.5607,49.0334}};loop11.AddElem(l1_41);
DefSeg l2_41{{103.5607,49.0334},{107.3231,48.7676}};loop11.AddElem(l2_41);
DefSeg l3_41{{107.3231,48.7676},{106.3631,40.1333}};loop11.AddElem(l3_41);
DefSeg l4_41{{106.3631,40.1333},{124.8341,38.0646}};loop11.AddElem(l4_41);
DefSeg l5_41{{124.8341,38.0646},{123.1039,34.761}};loop11.AddElem(l5_41);region.AddILoop(loop11);
ofts::DefLoop loop12;
DefSeg l0_5{{98.1742,59.2136},{119.3641,57.438}};loop12.AddElem(l0_5);
DefSeg l1_5{{119.3641,57.438},{118.6489,53.1496}};loop12.AddElem(l1_5);
DefSeg l2_5{{118.6489,53.1496},{97.7747,54.7491}};loop12.AddElem(l2_5);
DefSeg l3_5{{97.7747,54.7491},{98.1742,59.2136}};loop12.AddElem(l3_5);region.AddILoop(loop12);

for(auto& l : region._vIslLoop){
    l.SetLeftward(true);
}

/*DefSeg l1{{},{,}};
DefSeg l1{{},{,}};
DefArc arc0{{},{,},{,},,true};
Point pnt(,);
DefCircle circle_(Point(,),);*/

}

void CloseLoopData::ParallelData(ofts::GeomArea& region)
{
    DefSeg l0{{49.1056,50.3322},{42.407,178.774}};region._bndLoop.AddElem(l0);
    DefSeg l1{{42.407,178.774},{83.0168,228.9792}};region._bndLoop.AddElem(l1);
    DefSeg l2{{83.0168,228.9792},{126.5301,241.3461}};region._bndLoop.AddElem(l2);
    DefSeg l3{{126.5301,241.3461},{199.3566,219.0005}};region._bndLoop.AddElem(l3);
    DefSeg l4{{199.3566,219.0005},{261.6825,241.0075}};region._bndLoop.AddElem(l4);
    DefSeg l5{{261.6825,241.0075},{355.9923,225.3887}};region._bndLoop.AddElem(l5);
    DefSeg l6{{355.9923,225.3887},{372.9665,177.5775}};region._bndLoop.AddElem(l6);
    DefSeg l7{{372.9665,177.5775},{334.3716,172.9376}};region._bndLoop.AddElem(l7);
    DefSeg l8{{334.3716,172.9376},{289.1025,191.4971}};region._bndLoop.AddElem(l8);
    DefSeg l9{{289.1025,191.4971},{285.655,183.088}};region._bndLoop.AddElem(l9);
    DefSeg l10{{285.655,183.088},{351.2791,153.2223}};region._bndLoop.AddElem(l10);
    DefSeg l11{{351.2791,153.2223},{345.3987,128.5689}};region._bndLoop.AddElem(l11);
    DefSeg l12{{345.3987,128.5689},{271.9815,146.5484}};region._bndLoop.AddElem(l12);
    DefSeg l13{{271.9815,146.5484},{262.5171,115.6422}};region._bndLoop.AddElem(l13);
    DefSeg l14{{262.5171,115.6422},{319.0944,113.6109}};region._bndLoop.AddElem(l14);
    DefSeg l15{{319.0944,113.6109},{318.5616,65.0184}};region._bndLoop.AddElem(l15);
    DefSeg l16{{318.5616,65.0184},{289.8918,63.6208}};region._bndLoop.AddElem(l16);
    DefSeg l17{{289.8918,63.6208},{251.0881,84.4901}};region._bndLoop.AddElem(l17);
    DefSeg l18{{251.0881,84.4901},{240.4827,73.3951}};region._bndLoop.AddElem(l18);
    DefSeg l19{{240.4827,73.3951},{273.8087,46.8497}};region._bndLoop.AddElem(l19);
    DefSeg l20{{273.8087,46.8497},{296.1852,34.2714}};region._bndLoop.AddElem(l20);
    DefSeg l21{{296.1852,34.2714},{272.112,6.359}};region._bndLoop.AddElem(l21);
    DefSeg l22{{272.112,6.359},{99.3568,0.5964}};region._bndLoop.AddElem(l22);
    DefSeg l23{{99.3568,0.5964},{49.1056,50.3322}};region._bndLoop.AddElem(l23);

    DefLoop loop0,loop1,loop2,loop3,loop4,loop5,loop6,
            loop7,loop8,loop9,loop10,loop11,loop12,loop13;

    DefCircle c0({210.125,169.2389},15.6343);loop0.AddElem(c0);
    DefCircle c1({257.0895,163.6111},9.3142);loop1.AddElem(c1);
    DefCircle c2({327.35,146.3712},6.4508);loop2.AddElem(c2);
    DefCircle c3({236.8262,99.4738},10.0275);loop3.AddElem(c3);
    DefCircle c4({296.6173,84.9344},10.4897);loop4.AddElem(c4);
    DefCircle c5({105.4547,78.5545},16.7695);loop5.AddElem(c5);
    DefCircle c6({207.7846,125.476},12.3981);loop6.AddElem(c6);
    DefCircle c7({165.2315,104.547},14.3564);loop7.AddElem(c7);

    DefSeg l0_8{{169.8837,205.6245},{162.3187,190.8532}};loop8.AddElem(l0_8);
    DefSeg l1_8{{162.3187,190.8532},{144.5497,205.6245}};loop8.AddElem(l1_8);
    DefSeg l2_8{{144.5497,205.6245},{158.976,218.2856}};loop8.AddElem(l2_8);
    DefSeg l3_8{{158.976,218.2856},{169.8837,205.6245}};loop8.AddElem(l3_8);

    DefSeg l0_9{{147.8087,166.4885},{170.9293,166.2067}};loop9.AddElem(l0_9);
    DefSeg l1_9{{170.9293,166.2067},{152.602,157.7519}};loop9.AddElem(l1_9);
    DefSeg l2_9{{152.602,157.7519},{165.2901,146.197}};loop9.AddElem(l2_9);
    DefSeg l3_9{{165.2901,146.197},{146.9629,152.3972}};loop9.AddElem(l3_9);
    DefSeg l4_9{{146.9629,152.3972},{155.4216,134.0784}};loop9.AddElem(l4_9);
    DefSeg l5_9{{155.4216,134.0784},{139.068,144.506}};loop9.AddElem(l5_9);
    DefSeg l6_9{{139.068,144.506},{122.7145,140.5604}};loop9.AddElem(l6_9);
    DefSeg l7_9{{122.7145,140.5604},{128.9175,151.5517}};loop9.AddElem(l7_9);
    DefSeg l8_9{{128.9175,151.5517},{107.2068,156.3427}};loop9.AddElem(l8_9);
    DefSeg l9_9{{107.2068,156.3427},{124.9701,162.8247}};loop9.AddElem(l9_9);
    DefSeg l10_9{{124.9701,162.8247},{108.6166,174.3796}};loop9.AddElem(l10_9);
    DefSeg l11_9{{108.6166,174.3796},{127.7897,169.8704}};loop9.AddElem(l11_9);
    DefSeg l12_9{{127.7897,169.8704},{117.6392,193.262}};loop9.AddElem(l12_9);
    DefSeg l13_9{{117.6392,193.262},{138.5041,174.9433}};loop9.AddElem(l13_9);
    DefSeg l14_9{{138.5041,174.9433},{141.7016,187.1055}};loop9.AddElem(l14_9);
    DefSeg l15_9{{141.7016,187.1055},{148.0907,177.1979}};loop9.AddElem(l15_9);
    DefSeg l16_9{{148.0907,177.1979},{156.8314,182.8344}};loop9.AddElem(l16_9);
    DefSeg l17_9{{156.8314,182.8344},{147.8087,166.4885}};loop9.AddElem(l17_9);

    DefSeg l0_10{{87.4952,141.636},{103.2679,128.4345}};loop10.AddElem(l0_10);
    DefSeg l1_10{{103.2679,128.4345},{95.6607,113.213}};loop10.AddElem(l1_10);
    DefSeg l2_10{{95.6607,113.213},{79.871,121.1042}};loop10.AddElem(l2_10);
    DefSeg l3_10{{79.871,121.1042},{87.4952,141.636}};loop10.AddElem(l3_10);

    DefCircle c11({138.2255,43.0763},13.8475);loop11.AddElem(c11);

    DefSeg l0_12{{196.6515,49.4901},{175.375,63.6678}};loop12.AddElem(l0_12);
    DefSeg l1_12{{175.375,63.6678},{189.2216,77.1704}};loop12.AddElem(l1_12);
    DefSeg l2_12{{189.2216,77.1704},{210.0478,69.5939}};loop12.AddElem(l2_12);
    DefSeg l3_12{{210.0478,69.5939},{196.6515,49.4901}};loop12.AddElem(l3_12);

    DefCircle c13({237.1782,36.325},10.6016);loop13.AddElem(c13);
    region.SetILoops(std::vector<DefLoop>{loop0,loop1,loop2,loop3,loop4,loop5,loop6,loop7,
                                          loop8,loop9,loop10,loop11,loop12,loop13});

}





















