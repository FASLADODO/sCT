#!/bin/usr

import os
import time
import shutil

patienter_path = "C:/Users/chris_000/sCT/src/test_patienter"
mask_name = "mask.nii"
ct_name = "ct.nii"
coreg_exe = os.path.join("C:/Users/chris_000/sCT/src/coreg/build/Release", "CoReg.exe")
coreg_folder = "coreg"


def coreg_without_mask(fixed_image, moving_image, output_path, mask_path):
    os.system(os.path.normpath(coreg_exe) + " \""
                + os.path.normpath(fixed_image) + "\" \""
                + os.path.normpath(moving_image) + "\" \""  
                + os.path.normpath(output_path) + "\" \""  
                + os.path.normpath(mask_path) + "\" \"true\"" )

def coreg_with_mask(fixed_image, moving_image, output_path, mask_path):
    os.system(os.path.normpath(coreg_exe) + " \""
                + os.path.normpath(fixed_image) + "\" \""
                + os.path.normpath(moving_image) + "\" \""  
                + os.path.normpath(output_path) + "\" \""  
                + os.path.normpath(mask_path) + "\"" )

def coreg():
    patienter = os.listdir(patienter_path)
    for patient in patienter:
        if  os.path.isdir(patient):

            # Handle the odd case where folder name differ
            if "020939" in patient:
                pet_folder = "Petmr Hjerne_Fet"
            else:
                pet_folder = "Petmr Hjerne_Fdg"

            # Set the path to the patient
            patient_path = os.path.join(patienter_path, patient)
            # Find the images for the patient
            billed_mapper = os.listdir(os.path.join(patient_path, pet_folder))
            
            # Set the fixed image as the first image
            fixed_image_folder  = billed_mapper.pop(0)
            fixed_image         = os.listdir(os.path.join(patienter_path, patient, pet_folder, fixed_image_folder))[0]
            fixed_image_path    = os.path.join(patienter_path, patient, pet_folder, fixed_image_folder, fixed_image)
            
            # Set the output path
            output_path = os.path.join(patient_path, coreg_folder)
            
            # Set the mask path
            mask_path = os.path.join(output_path, mask_name)
            
            # Remove old output folder if it exists
            if os.path.exists(output_path):
                shutil.rmtree(output_path)
            
            # Make the folder structure and copy of the fixed image
            os.mkdir(os.path.normpath(output_path))
            os.mkdir(os.path.normpath(os.path.join(output_path, fixed_image_folder)))
            shutil.copy(os.path.normpath(fixed_image_path), os.path.normpath(os.path.join(output_path, fixed_image_folder)))
            
            # Get the first moving image
            moving_image_folder = billed_mapper.pop(0)
            moving_image = os.listdir(os.path.join(patienter_path, patient, pet_folder, moving_image_folder))[0]
            moving_image_path = os.path.join(patienter_path, patient, pet_folder, moving_image_folder, moving_image)
            os.mkdir(os.path.normpath(os.path.join(output_path, moving_image_folder)))
            
            # Compute the first moving image including its mask
            coreg_without_mask(fixed_image_path, moving_image_path, os.path.join(output_path, moving_image_folder, moving_image), mask_path)

            # Compute the rest of the images
            for billed in billed_mapper:
                moving_image_folder = billed
                moving_image = os.listdir(os.path.join(patienter_path, patient, pet_folder, moving_image_folder))[0]
                moving_image_path = os.path.join(patienter_path, patient, pet_folder, moving_image_folder, moving_image)
                os.mkdir(os.path.normpath(os.path.join(output_path, moving_image_folder)))
                coreg_with_mask(fixed_image_path, moving_image_path, os.path.join(output_path, moving_image_folder, moving_image), mask_path)
            
            # Compute the ct image
            moving_image = ct_name
            moving_image_path = os.path.join(patienter_path, patient, moving_image)
            coreg_with_mask(fixed_image_path, moving_image_path, os.path.join(output_path, moving_image), mask_path)

coreg()