cd tmp/Automated-GuitarAmpModelling

cp ../../res/LSTM-16.json Configs/LSTM-16.json

rm -r -f Results/amp_*

python3 prep_wav.py -f ../../dataset/input.wav  ../../dataset/output.wav -l LSTM-16.json -n

python3 dist_model_recnet.py -l LSTM-16.json -slen 24000 --seed 39 -lm 0
