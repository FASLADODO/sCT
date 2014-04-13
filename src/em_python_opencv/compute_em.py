import nibabel as nii
import numpy as np
import cv2

nii1 = nii.load('D:/sCT/src/em_python_opencv/coreg/ct.nii').get_data();
nii2 = nii.load('D:/sCT/src/em_python_opencv/coreg/Head_CV_UTE_AC_WIP_719 - 13.nii').get_data();
nii3 = nii.load('D:/sCT/src/em_python_opencv/coreg/Head_CV_UTE_AC_WIP_719 - 14.nii').get_data();
nii4 = nii.load('D:/sCT/src/em_python_opencv/coreg/Head_CV_UTE_AC_WIP_719_18deg - 18.nii').get_data();
nii5 = nii.load('D:/sCT/src/em_python_opencv/coreg/Head_CV_UTE_AC_WIP_719_18deg - 19.nii').get_data();
nii6 = nii.load('D:/sCT/src/em_python_opencv/coreg/Head_t1_mprage_sag_p2_iso - 7.nii').get_data();
nii7 = nii.load('D:/sCT/src/em_python_opencv/coreg/mean_Head_CV_UTE_AC_WIP_719 - 13.nii').get_data();
nii8 = nii.load('D:/sCT/src/em_python_opencv/coreg/mean_Head_CV_UTE_AC_WIP_719 - 14.nii').get_data();
nii9 = nii.load('D:/sCT/src/em_python_opencv/coreg/mean_Head_CV_UTE_AC_WIP_719_18deg - 18.nii').get_data();
nii10 = nii.load('D:/sCT/src/em_python_opencv/coreg/mean_Head_CV_UTE_AC_WIP_719_18deg - 19.nii').get_data();
nii11 = nii.load('D:/sCT/src/em_python_opencv/coreg/mean_Head_t1_mprage_sag_p2_iso - 7.nii').get_data();
nii12 = nii.load('D:/sCT/src/em_python_opencv/coreg/variance_Head_CV_UTE_AC_WIP_719 - 13.nii').get_data();
nii13 = nii.load('D:/sCT/src/em_python_opencv/coreg/variance_Head_CV_UTE_AC_WIP_719 - 14.nii').get_data();
nii14 = nii.load('D:/sCT/src/em_python_opencv/coreg/variance_Head_CV_UTE_AC_WIP_719_18deg - 18.nii').get_data();
nii15 = nii.load('D:/sCT/src/em_python_opencv/coreg/variance_Head_CV_UTE_AC_WIP_719_18deg - 19.nii').get_data();
nii16 = nii.load('D:/sCT/src/em_python_opencv/coreg/variance_Head_t1_mprage_sag_p2_iso - 7.nii').get_data();

mask = nii.load('D:/sCT/src/em_python_opencv/coreg/mask.nii').get_data();

nii_list = np.zeros([192*192*192, 16],np.float32);
count = 0;

for i in range(192):
    for j in range(192):
        for k in range(192):
            if mask[i, j, k] == 255:
               nii_list[count, 0]  = np.float32(nii1[i, j, k])
               nii_list[count, 1]  = np.float32(nii2[i, j, k])
               nii_list[count, 2]  = np.float32(nii3[i, j, k])
               nii_list[count, 3]  = np.float32(nii4[i, j, k])
               nii_list[count, 4]  = np.float32(nii5[i, j, k])
               nii_list[count, 5]  = np.float32(nii6[i, j, k])
               nii_list[count, 6]  = np.float32(nii7[i, j, k])
               nii_list[count, 7]  = np.float32(nii8[i, j, k])
               nii_list[count, 8]  = np.float32(nii9[i, j, k])
               nii_list[count, 9]  = np.float32(nii10[i, j, k])
               nii_list[count, 10] = np.float32(nii11[i, j, k])
               nii_list[count, 11] = np.float32(nii12[i, j, k])
               nii_list[count, 12] = np.float32(nii13[i, j, k])
               nii_list[count, 13] = np.float32(nii14[i, j, k])
               nii_list[count, 14] = np.float32(nii15[i, j, k])
               nii_list[count, 15] = np.float32(nii16[i, j, k])
               count += 1

nii_list = nii_list[range(count-1), :]

em = cv2.EM(20)

em.train(nii_list)

weights = em.getMat('weights')

covs = em.getMatVector('covs')

means = em.getMat('means')

with open('parameters.json', 'w') as f:
    f.write(json.dumps({'means': means.tolist(), 'covs': covs, 'weights': weights.tolist()}))