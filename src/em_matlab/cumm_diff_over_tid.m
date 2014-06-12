cumm_diff_v1 = cumm_diff('011030', 'early_sct.nii');
cumm_diff_v2 = cumm_diff('011030', 'middle_sct.nii');
cumm_diff_v3 = cumm_diff('011030', 'late_sct.nii');

figure1 = figure;
axes1 = axes('Parent',figure1,'YGrid','on','XGrid','on',...
    'Position',[0.0345092838196287 0.124084507042254 0.775 0.815]);
xlim(axes1,[-400 400]);
box(axes1,'on');
hold(axes1,'all');

plot1 = plot(cumm_diff_v1(:,1), cumm_diff_v1(:,2),...
    cumm_diff_v2(:,1), cumm_diff_v2(:,2),...
    cumm_diff_v3(:,1), cumm_diff_v3(:,2),...
    'Parent',axes1,'Color',[0 0 0]);
set(plot1(1),'DisplayName','Early sCT');
set(plot1(2),'LineStyle','--','DisplayName','Middle sCT');
set(plot1(3),'LineWidth',2,'LineStyle',':','DisplayName','Late sCT');

% Create legend
legend1 = legend(axes1,'show');
set(legend1,'EdgeColor',[1 1 1],'Location','NorthWest','YColor',[1 1 1],...
    'XColor',[1 1 1]);