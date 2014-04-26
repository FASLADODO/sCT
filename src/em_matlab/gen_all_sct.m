tic;
[p0,p1,p2,p3,p4,p5,p6,p7,p8,p9] = load_all_patients_func();
toc;

tic;
sct0 = gen_sct_func( p0, '010476/coreg/mask.nii');
save('sct0.nii','sct0');
toc;

tic;
sct1 = gen_sct_func( p1, '010769/coreg/mask.nii');
save('sct1.nii','sct1');
toc;

tic;
sct2 = gen_sct_func( p2, '010850/coreg/mask.nii');
save('sct2.nii','sct2');
toc;

tic;
sct3 = gen_sct_func( p3, '010960/coreg/mask.nii');
save('sct3.nii','sct3');
toc;

tic;
sct4 = gen_sct_func( p4, '011030/coreg/mask.nii');
save('sct4.nii','sct4');
toc;

tic;
sct5 = gen_sct_func( p5, '020879/coreg/mask.nii');
save('sct5.nii','sct5');
toc;

tic;
sct6 = gen_sct_func( p6, '020939/coreg/mask.nii');
save('sct6.nii','sct6');
toc;

tic;
sct7 = gen_sct_func( p7, '030242/coreg/mask.nii');
save('sct7.nii','sct7');
toc;

tic;
sct8 = gen_sct_func( p8, '030330/coreg/mask.nii');
save('sct8.nii','sct8');
toc;

tic;
sct9 = gen_sct_func( p9, '040145/coreg/mask.nii');
save('sct9.nii','sct9');
toc;
