sh tasks/build_nam_converter.sh
echo ">> Converting NAM profile"

./build/NamReamp res/0.nam res/input.wav res/output.wav

echo ">> Profile converted"

mkdir -p dataset
cp res/input.wav dataset/input.wav
cp res/output.wav dataset/output.wav

sh tasks/prepare_aida_trainer.sh
sh tasks/install_python_packages.sh
sh tasks/convert_to_aida.sh