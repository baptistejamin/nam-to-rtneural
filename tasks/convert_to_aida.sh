cd tmp/Automated-GuitarAmpModelling

cp ../../res/LSTM-16.json Configs/LSTM-16.json

rm -r -f Results/amp_*

python3 prep_wav.py -f ../../dataset/input.wav  ../../dataset/output.wav -l LSTM-16.json -n

python3 dist_model_recnet.py -l LSTM-16.json -slen 24000 --epoch 200 --seed 39 -lm 0

cp Results/amp_LSTM-16-1/best_model.json ../../res/model-lstm.nam

python3 simple_modelToKeras.py --inputfile Results/amp_LSTM-16-1/model_best.json --outputfile ../../res/model-lstm.aidax

Echo "res/model-lstm.aidax and model-lstm.nam have been generated"