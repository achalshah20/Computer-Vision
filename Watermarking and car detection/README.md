# achshah-lijguo-ssable-kkokje-a1

##Part 1:

Compile: Run makefile to get watermark executable

**1.1 Generate spectrogram of noise1.png**  
./watermark 1.1 input/noise1.png output/1.1/spect_1.1.png  

Output file: output/1.1/spect_1.1.png  

**1.2 Remove noise from noise1.png and generate spectrogram of clean image**  
./watermark 1.2 input/noise1.png output/1.2/clean.png  

Output files:  
output/1.2/clean.png  
output/1.2/spect_after_noise_removal.png  

**1.3 Add and check watermark for different images**  
Qualitative analysis: Tested on 4 different images from input directory  
Quantitative analysis: Tested for 100 different numbers  

Correlation threshold: 0.20  
Accuracy: 100%  
./evaluate_part1.sh   

Output files:  
All watermarked images and their spectrogram will be in output/1.3  

## Part 2 Car detection
We have two implementations of car detection program.

1. The 'part2' folder contains c++ implementation. Results are described in 'Assignment1_report.docx.pdf'.

2. The 'part2_car_matlab' folder contains Matlab implementation. Algorithms and results are described in 'Assignment1_report_car_appendix.pdf'. Output images and location text files are in 'part2_car_matlab/output' folder.

