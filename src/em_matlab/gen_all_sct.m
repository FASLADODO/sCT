startup;

tic;
[p0,p1,p2,p3,p4,p5,p6,p7,p8,p9] = load_all_patients_func();
toc;

gen_sct_func( p0, '010476/coreg/mask.nii','010476/coreg/ct.nii', 'sct0.nii');
toc;

gen_sct_func( p1, '010769/coreg/mask.nii','010769/coreg/ct.nii', 'sct1.nii');
toc;

gen_sct_func( p2, '010850/coreg/mask.nii','010850/coreg/ct.nii', 'sct2.nii');
toc;

gen_sct_func( p3, '010960/coreg/mask.nii','010960/coreg/ct.nii', 'sct3.nii');
toc;

gen_sct_func( p4, '011030/coreg/mask.nii','011030/coreg/ct.nii', 'sct4.nii');
toc;

gen_sct_func( p5, '020879/coreg/mask.nii','020879/coreg/ct.nii', 'sct5.nii');
toc;

gen_sct_func( p6, '020939/coreg/mask.nii','020939/coreg/ct.nii', 'sct6.nii');
toc;

gen_sct_func( p7, '030242/coreg/mask.nii','030242/coreg/ct.nii', 'sct7.nii');
toc;

gen_sct_func( p8, '030330/coreg/mask.nii','030330/coreg/ct.nii', 'sct8.nii');
toc;

gen_sct_func( p9, '040145/coreg/mask.nii','040145/coreg/ct.nii', 'sct9.nii');
toc;
