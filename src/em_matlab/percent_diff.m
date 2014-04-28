patient = '010476';
ct = load_nii(strcat(patient,'/coreg/ct.nii'));
sct = load_nii(strcat(patient,'/coreg/sct.nii'));
ct_max = max( reshape( ct.img, numel( ct.img ), 1 ) );
ct_norm = ct.img ./ ct_max;
sct_max = max( reshape( sct.img, numel( sct.img ), 1 ) );
sct_norm = sct.img ./ sct_max;

pdct = ( abs( (ct.img) - (sct.img) ) ./ ( ct.img + sct.img )./2 ) .* 100;

i = find(pdct > 100);
pdct(i) = 100;

i = find(pdct < -100);
pdct(i) = -100;

save_nii(make_nii(pdct), 'pdct.nii');
