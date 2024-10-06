cd tmp/Automated-GuitarAmpModelling

rm -r -f Results/amp_*
rm -r -f TensorboardData/amp_*
rm -r -f Data/test/*
rm -r -f Data/val/*
rm -r -f Data/train/*

cp ../../python/train.py train.py

python3 train.py

cp amp_LSTM-16-0.json ../../res/model-lstm.aidax
cp Results/amp_LSTM-16-0/model_best.json ../../res/model-lstm.nam

echo "res/model-lstm.aidax model-lstm.nam have been generated"