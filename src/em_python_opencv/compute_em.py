import nibabel as nii
import numpy as np
import cv2
import time
import json

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

start_time = time.time()

nii_list = np.array([nii1[mask.nonzero()].flatten(),
                     nii2[mask.nonzero()].flatten(),
                     nii3[mask.nonzero()].flatten(),
                     nii4[mask.nonzero()].flatten(),
                     nii5[mask.nonzero()].flatten(),
                     nii6[mask.nonzero()].flatten(),
                     nii7[mask.nonzero()].flatten(),
                     nii8[mask.nonzero()].flatten(),
                     nii9[mask.nonzero()].flatten(),
                     nii10[mask.nonzero()].flatten(),
                     nii11[mask.nonzero()].flatten(),
                     nii12[mask.nonzero()].flatten(),
                     nii13[mask.nonzero()].flatten(),
                     nii14[mask.nonzero()].flatten(),
                     nii15[mask.nonzero()].flatten(),
                     nii16[mask.nonzero()].flatten()]).T

print "List", time.time() - start_time, "seconds"

start_time = time.time()

em = cv2.EM()
em.setInt("nclusters", 20)
em.setInt("covMatType", cv2.EM_COV_MAT_GENERIC)

em.train(nii_list)

print "EM", time.time() - start_time, "seconds"

weights = em.getMat('weights')

covs = em.getMatVector('covs')

means = em.getMat('means')

with open('parameters.json', 'w') as f:
    f.write(json.dumps({'means': means.tolist(), 'covs': [covs[x].tolist() for x in range(len(covs))], 'weights': weights.tolist()}))