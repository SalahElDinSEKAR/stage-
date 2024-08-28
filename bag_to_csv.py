
import pandas as pd
from rosbags.rosbag2 import Reader
from rosbags.typesys import Stores, get_typestore

def bag_to_csv(bag_folder, csv_folder):
    typestore = get_typestore(Stores.ROS2_HUMBLE)  # Utiliser le typestore pour ROS 2 Humble
    with Reader(bag_folder) as reader:
        reader.open()
        for connection in reader.connections:
            topic = connection.topic
            data = []
            for _, timestamp, rawdata in reader.messages(connections=[connection]):
                msg = typestore.deserialize_cdr(rawdata, connection.msgtype)
                row = [timestamp / 1e9]  # Convert timestamp to seconds
                if connection.msgtype == 'std_msgs/msg/Float64MultiArray':
                    row.extend(msg.data)
                elif connection.msgtype == 'std_msgs/msg/Float64':
                    row.append(msg.data)
                data.append(row)

            if connection.msgtype in ['std_msgs/msg/Float64MultiArray', 'std_msgs/msg/Float64']:
                if connection.msgtype == 'std_msgs/msg/Float64MultiArray':
                    columns = ['time'] + [f'data{i}' for i in range(len(msg.data))]
                elif connection.msgtype == 'std_msgs/msg/Float64':
                    columns = ['time', 'data']
                df = pd.DataFrame(data, columns=columns)
                df.to_csv(f"{csv_folder}/{topic.replace('/', '_')}.csv", index=False)

bag_folder = 'rosbag2_2024_05_24-16_08_43'  # Chemin du dossier contenant le fichier .db3 et metadata.yaml
csv_folder = 'Info_for_traces'
bag_to_csv(bag_folder, csv_folder)