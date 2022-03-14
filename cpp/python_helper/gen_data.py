import json
from math import sin, cos, floor

def main():
    steps = 1000
    distance = 100
    data = {
        'fish1': {
            'file': '12265_Fish_v1_L2',
            'info': 'Pelvicachromis pulcher is a freshwater fish of the '\
                    'cichlid family, endemic to Nigeria and Cameroon. '\
                    'It is popular amongst aquarium hobbyists, and '\
                    'is most commonly sold under the name kribensis, '\
                    'although it has other common names, including various '\
                    'derivatives and color morphs of the kribensis: krib, '\
                    'common krib, red krib, super-red krib and rainbow krib, '\
                    'along with rainbow cichlid and purple cichlid.',
            'path': [],
            'rotate_x': -90.0,
            'rotate_y': 90.0,
            'scale': 0.1
        },
        'fish2': {
            'file': 'fish',
            'info': 'Carp are various species of oily freshwater fish from the '\
                    'family Cyprinidae, a very large group of fish native to '\
                    'Europe and Asia.  While carp is consumed in many parts of'\
                    'the world, they are generally considered an invasive'\
                    'species in parts of Africa, Australia and most of the'\
                    'United States.',
            'path': [],
            'rotate_x': 0.0,
            'rotate_y': 0.0,
            'scale': 1.0
        },
        'fish3': {
            'file': '13007_Blue-Green_Reef_Chromis_v2_l3',
            'info': 'Chromis viridis (green chromis) is a species of '\
                    'damselfish. Individuals tend to be iridescent '\
                    'apple-green and light blue, and reach a maximal '\
                    'length of 10 cm. It is sometimes called the '\
                    '"blue-green chromis", but that may also refer to '\
                    'Chromis caerulea, the blue puller, a close relative '\
                    'that is sometimes considered conspecific.',
            'path': [],
            'rotate_x': -90.0,
            'rotate_y': 0.0,
            'scale': 0.5
        }
    }
    maps = {
            'fish1': lambda i: [3*cos(i / steps * 50) - 10,
                3*sin(i / steps * 50), distance / steps * i],
            'fish2': lambda i: [0, 10 * abs(i / 50 - floor(i / 50 + 0.5))
                - 2.5, distance / steps * i],
            'fish3': lambda i: [5*sin(i / steps * 100) + 10, 0,
                distance / steps * i]
    }
    for fish in data.keys():
        for i in range(steps):
            data[fish]['path'].append(maps[fish](i))
    json.dump(data, open('fish_data.json', 'w'))

if __name__ == '__main__':
    main()
