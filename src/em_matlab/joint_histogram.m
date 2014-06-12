startup;

patient = '210445';
ct = load_nii(strcat('over_tid/',patient,'/ct.nii'));
sct = load_nii(strcat('over_tid/',patient,'/earlysct_rsl.nii'));
mask = load_nii(strcat('over_tid/',patient,'/mask_air_inv.nii'));
maskimg = mask.img(:);

ctimg = int16(ct.img(maskimg > 0)) + 1024;
sctimg = int16(sct.img(maskimg > 0)) + 1024;

jh = zeros(max(ctimg) - min(ctimg )+ 10000, max(sctimg) - min(sctimg) + 10000);

for z = 1:length(ctimg)
    x = ctimg(z)+10000;
    y = sctimg(z)+10000;
    jh(x, y) = jh(x, y) + 1;
end

c = zeros(length(ctimg), 1);

for z = 1:length(ctimg)
    x = ctimg(z)+10000;
    y = sctimg(z)+10000;
    c(z) = jh(x, y);
end

w = 2^16/max(c);

c1 = c .* w;

cl = log(c1);

w1 = 2^8/max(cl);

c2 = cl .* w1;

cf = uint8(c2);

cf(1) = 1;

% Create figure
figure2 = figure('PaperSize',[20.98404194812 29.67743169791]);
set(figure2, 'renderer', 'OpenGL')

% Create axes
axes1 = axes('Parent',figure2,'Color',[0 0 0]);
box(axes1,'on');
hold(axes1,'all');

%% Uncomment the following line to preserve the X-limits of the axes
%xlim(axes1,[-1000 1500]);
%% Uncomment the following line to preserve the Y-limits of the axes
%ylim(axes1,[-1000 1500]);

axis equal;

colormap('gray');

% Create plot
scatter(ctimg, sctimg, ones(length(cf), 1), cf, 'marker', '.');

