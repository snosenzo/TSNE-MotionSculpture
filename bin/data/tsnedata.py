import tensorflow as tf

filename = "word2vec_10000_200d_tensors.bytes"

reader = tf.FixedLengthRecordReader(record_bytes=800)

val = reader.read(filename);
print(val);