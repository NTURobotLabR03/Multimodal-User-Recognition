# Multimodal-User-Recognition
User Recognition by Three Modalities: Face, Body Size and Limb Ratio, Dress Color Information

使用library: opencv 2.4.9, libsvm-3.16, fuzzylite, Kinect SDK<br>
開發環境: Microsoft Visual Studio 2010<br>
<br>
這是一個使用者識別程式<br>
內含三個不同的認別模組:<br>
人臉辨識<br>
身體長度及肢體比例<br>
衣服顏色比對<br>
<br>
利用fuzzy influence 把三個modality fusion起來<br>
最後得出fusion值，達到multimodal user recognition<br>
<br>
主執行檔放在main.cpp內，需輸入camera截取之影像、Kinect截取之骨架影像。<br>
