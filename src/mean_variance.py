#!/bin/usr

import os
import shutil

mean_gen_exe = "C:/Users/chris_000/sCT/src/mean_gen/build/Release/mean_gen.exe"
patient_folder = "C:/Users/chris_000/sCT/src/test_patienter"
coreg_folder = "coreg"

def calculate_variance(input, output):
    pass

def calculate_mean(input, output):
    calculate(mean_gen_exe, input, output)

def calculate(exe, input, output):
    os.system(os.path.normpath(exe) + " \""
                + os.path.normpath(input) + "\" \""
                + os.path.normpath(output) + "\"")

def run(patients):
    for patient in os.listdir(patients):
        if os.path.isdir(os.path.join(patients, patient)):
            for image in os.listdir(os.path.join(patients, patient, coreg_folder)):
                if "Head" in image and not "UMAP" in image:
                    input_path = os.path.join(patients, patient, coreg_folder, image)
                    mean_output_path = os.path.join(patients, patient, coreg_folder, "mean_" + image)
                    var_output_path = os.path.join(patients, patient, coreg_folder, "variance_" + image)
                    calculate_mean(input_path, mean_output_path)
                    calculate_variance(input_path, var_output_path)

run(patient_folder)