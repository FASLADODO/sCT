startup;

sctt1 = load_nii('ct1.nii');
sctt2 = load_nii('sct0t2.nii');

sctt1 = sctt1.img();
sctt2 = sctt2.img();

% Create figure
figure1 = figure('PaperSize',[20.98404194812 29.67743169791]);
set(figure1, 'renderer', 'OpenGL')

% Create axes
axes1 = axes('Parent',figure1,'Color',[0 0 0],'CLim',[0 1]);
box(axes1,'on');
hold(axes1,'all');

% Create plot
plot(sctt1(:),sctt2(:),'Parent',axes1,'MarkerFaceColor',[1 1 1],...
    'MarkerEdgeColor',[1 1 1],...
    'MarkerSize',0.001,...
    'Marker','.',...
    'LineWidth',0.001,...
    'LineStyle','none');