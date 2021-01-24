
REM --load model_DQN.h5

python bombermaaan.py --train --replay-memory-size=5000 --target-network-update-freq=10 --avg-val-computation-freq=50 --replay-start-size=2000 --test-freq=2500 --validation-frames=150 --min-epsilon=0.01 --epsilon-decrease=0.01 --learning-rate=0.01

