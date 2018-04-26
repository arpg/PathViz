% reset state
clear; clc;

% USER INPUT ==============================================================

% car trajectory
path = zeros(100, 7);
path(:, 1) = 0 : size(path, 1) - 1;
path(:, 2) = 0 : size(path, 1) - 1;

% bounding box padding
padding = [ 5 ; 20 ; 20 ];

% bottom feature plane
Ib = uint8(zeros(20, 5));
Ib( 3, 3) = 1;
Ib(15, 2) = 1;

% top feature plane
It = uint8(zeros(20, 5));
It(2, 3) = 1;

% back feature plane
Ik = uint8(zeros(5, 5));
Ik( 2,  1) = 1;

% front feature plane
If = uint8(zeros(5, 5));
If( 4,  4) = 1;

% left feature plane
Il = uint8(zeros(20, 5));

% right feature plane
Ir = uint8(zeros(20, 5));
Ir(13, 3) = 1;
Ir( 5, 2) = 1;
Ir( 8, 1) = 1;

% landmarks per plane
samples = 30;

% RNG seed
rng(0);

%==========================================================================

% reset figure
clf; hold on; axis equal;

% compute bounds
bounds = zeros(3, 2);
bounds(:, 1) = min(path(:, 2:4))' - padding / 2;
bounds(:, 2) = max(path(:, 2:4))' + padding / 2;

% plot trajectory
plot3(path(:, 2), path(:, 3), path(:, 4), 'b-', 'LineWidth', 2);

% open output files
box_file = fopen('boxes.csv', 'w');
landmark_file = fopen('landmark.csv', 'w');

% plot bottom plane
[ ~, B, P ] = plotplane(Ib, bounds, 1, 2, 0, samples);
writeboxes(box_file, B);
writelandmarks(landmark_file, P);

% plot top plane
[ ~, B, P ] = plotplane(It, bounds, 1, 2, 1, samples);
writeboxes(box_file, B);
writelandmarks(landmark_file, P);

% plot back plane
[ ~, B, P ] = plotplane(Ik, bounds, 2, 3, 0, samples);
writeboxes(box_file, B);
writelandmarks(landmark_file, P);

% plot front plane
[ ~, B, P ] = plotplane(If, bounds, 2, 3, 1, samples);
writeboxes(box_file, B);
writelandmarks(landmark_file, P);

% plot left plane
[ ~, B, P ] = plotplane(Il, bounds, 1, 3, 0, samples);
writeboxes(box_file, B);
writelandmarks(landmark_file, P);

% plot right plane
[ ~, B, P ] = plotplane(Ir, bounds, 1, 3, 1, samples);
writeboxes(box_file, B);
writelandmarks(landmark_file, P);

% close output files
fclose(box_file);
fclose(landmark_file);
csvwrite('path.csv', path);