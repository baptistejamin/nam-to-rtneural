import torch
import os
import numpy as np
import librosa
from time import sleep
import shutil
from colab_functions import wav2tensor, extract_best_esr_model, prep_audio, create_csv_nam_v1_1_1
from CoreAudioML.networks import load_model
import CoreAudioML.miscfuncs as miscfuncs

# Check GPU availability
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print(f"Using device: {device}")

# Set up environment variables
os.environ['CUBLAS_WORKSPACE_CONFIG'] = ':4096:2'
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# Function to prepare data
def prepare_data(data_dir):
    i_aud, i_sr = librosa.load(os.path.join(data_dir, "input.wav"), sr=None, mono=True)
    t_aud, t_sr = librosa.load(os.path.join(data_dir, "output.wav"), sr=None, mono=True)
    
    assert t_aud.shape[0]/t_sr - i_aud.shape[0]/i_sr < 3.0, "Input and Target audio files are not the same length!"
    assert i_sr == t_sr, "Input and Target audio files are not the same sample rate!"

    file_name = "amp"

    create_csv_nam_v1_1_1(os.path.join(data_dir, "input.wav") + ".csv")

    prep_audio([os.path.join(data_dir, "input.wav"), os.path.join(data_dir, "output.wav")],
               file_name=file_name, norm=True, csv_file=False)
    
    return file_name

# Function to train model
def train_model(file_name, model_type, skip_connection, epochs):
    config_file = {
        "Lightest": "LSTM-8",
        "Light": "LSTM-12",
        "Standard": "LSTM-16",
        "Heavy": "LSTM-20"
    }[model_type]
    
    skip_con = 1 if skip_connection == "ON" else 0
    
    print(f"python3 dist_model_recnet.py -l {config_file} -fn {file_name} -sc {skip_con} -eps {epochs}")
    
    # Call the training script (assuming it's available in the current directory)
    os.system(f"python3 dist_model_recnet.py -l {config_file} -fn {file_name} -sc {skip_con} -eps {epochs}")
    
    sleep(1)
    model_dir = f"Results/{file_name}_{config_file}-{skip_con}"
    return model_dir

# Function to export model
def export_model(model_dir):
    model_filename = os.path.split(model_dir)[-1] + '.json'
    print("Generating model file:", model_filename)
    
    model_path = os.path.join(model_dir, 'model_best.json')
    os.system(f"python3 modelToKeras.py -lm {model_path}")
    
    shutil.copyfile(os.path.join(model_dir, 'model_keras.json'), model_filename)
    print(f"Model file saved as: {model_filename}")

# Main execution
if __name__ == "__main__":
    #DATA_DIR = input("Enter the path to your data directory: ")
    DATA_DIR = "../../dataset/"
    file_name = prepare_data(DATA_DIR)
    
    #model_type = input("Choose model type (Lightest/Light/Standard/Heavy): ")
    model_type = "Standard"
    #skip_connection = input("Skip connection (ON/OFF): ")
    skip_connection = "OFF"
    #epochs = int(input("Number of epochs: "))
    epochs = 200
    
    model_dir = train_model(file_name, model_type, skip_connection, epochs)
    export_model(model_dir)