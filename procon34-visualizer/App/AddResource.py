import glob

def output_resource(path):
    print("Resource("+path+")")

for path in glob.glob("./images/*.png"):
    output_resource(path)
for path in glob.glob("./match_jsons/*.json"):
    output_resource(path)
for path in glob.glob("./field/*.csv"):
    output_resource(path)
