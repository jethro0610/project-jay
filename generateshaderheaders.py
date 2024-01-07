import os

vertex_filesnames = os.listdir('./Source/Shaders/Vertex/')
vertex_header = open('./Source/Resource/VertexShaders.h', 'w')
vertex_header.write('#pragma once\n')
vertex_header.write('const int NUM_VERTEX_SHADERS = %d;\n' % len(vertex_filesnames))
vertex_header.write('const char VERTEX_SHADERS[NUM_VERTEX_SHADERS][32] = {\n')
for filename in vertex_filesnames:
    vertex_header.write('\t\"' + filename[:-3] + '\",\n')
vertex_header.write('};\n')

fragment_filesnames = os.listdir('./Source/Shaders/Fragment/')
fragment_header = open('./Source/Resource/FragmentShaders.h', 'w')
fragment_header.write('#pragma once\n')
fragment_header.write('const int NUM_FRAGMENT_SHADERS = %d;\n' % len(fragment_filesnames))
fragment_header.write('const char FRAGMENT_SHADERS[NUM_FRAGMENT_SHADERS][32] = {\n')
num_fragment_shaders = 0

for filename in fragment_filesnames:
    num_fragment_shaders = num_fragment_shaders + 1 
    fragment_header.write('\t\"' + filename[:-3] + '\",\n')

fragment_header.write('};\n')
