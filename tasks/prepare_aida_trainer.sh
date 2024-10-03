mkdir -p tmp
if [ ! -d "tmp/Automated-GuitarAmpModelling" ]; then
  git clone --recurse-submodules https://github.com/AidaDSP/Automated-GuitarAmpModelling tmp/Automated-GuitarAmpModelling
  cd tmp/Automated-GuitarAmpModelling
  git checkout aidadsp_devel
  git submodule update --init --recursive
  cd ../..
fi
